#include <types/system/console/console.hpp>

#include <types/general/ascii.hpp>

#include <types/operators.hpp>


const char* ascii_icon[16] = {
    " XX             ",
    "  XXXX      X   ",
    "   XXX          ",
    "    XXX   XX    ",
    "    XXX  XX     ",
    "XX   XXX  X     ",
    " XX   XX  X   XX",
    "   X  XX  X XX  ",
    "   X  XX   XXX  ",
    "    X X X XXX   ",
    "    XXXXXX X    ",
    "     XXXXXX     ",
    "      XXXXX     ",
    "      XXXX      ",
    "      XXXX      ",
};


namespace Console
{
    bool IsStringEqual(
        IOBuffer::Cursor& start,
        const IOBuffer::Cursor& end,
        const char* test_str
    )
    {
        if (not start.IsValid() or test_str == nullptr)
        {
            return false;
        }
        auto i = 0;
        do
        {
            if (start.Item() != test_str[i])
            {
                return false;
            }
            ++i;
            ++start;
        }
        while(start != end);
        return test_str[i] == '\0';
    }

    Console::Console(HardwareSerial& serial_block, bool verbose_codes)
    : _serial(serial_block), _input_buffer(), _output_buffer(), _verbose_codes(verbose_codes)
    {
    }

    Console::~Console()
    {
        DeInit();
    }

    void Console::Init(uint32_t baud_rate)
    {
        _serial.begin(baud_rate);
        _output_buffer.InsertNewLine();

        if (Options::PrintBanner)
        {
            const auto topbot = "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|";
            const auto spacer = "|                              |";
            const auto naming = "|           fuckOS             |";
            _output_buffer.InsertString(topbot, true);
            _output_buffer.InsertString(spacer, true);
            _output_buffer.InsertString(naming, true);
            _output_buffer.InsertString(spacer, true);
            _output_buffer.InsertString(topbot, true);
        }
        else if (Options::PrintLogo)
        {
            for (auto line : ascii_icon)
            {
                _output_buffer.InsertString(line, true);
            }
            _output_buffer.InsertString("     fuckOS     ", true);
        }

        PrintCloseInput();
        FlushOutput();
    }

    void Console::DeInit()
    {
        _serial.end();
    }

    void Console::EventLoopStep(const uint32_t mils)
    {
        auto serial_char = _serial.read();
        if (serial_char < 0)
        {
            FlushOutput();
            return;
        }
        auto charin = static_cast<char>(serial_char);
        switch(charin)
        {
            case ASCII::ControlCodes::EndOfText:
                /* CTRL+C was invoked: clear local line buffer and create new console line */
                _input_buffer.GetBuffer().Discard();
                PrintControlCode(charin);
                PrintCloseInput();
                break;
            case ASCII::ControlCodes::CarriageReturn:
                // /* Ignore carriage return in Windows; not in for *NIX systems */
                // PrintControlCode(charin);
                // break;
            [[fallthrough]];
            case ASCII::ControlCodes::LineFeed:
                /* ENTER was pressed: invoke buffer parsing and processing */
                PrintControlCode(charin);
                ParseInputExecuteCommand();
                _input_buffer.GetBuffer().Discard();
                PrintCloseInput();
                break;
            case ASCII::ControlCodes::Backspace:
                /* Backspace: undo last push and echo that to console */
                _input_buffer.GetBuffer().RevertPush();
                _output_buffer << charin;
                break;
            default:
                /* Unhandled control codes + printable chars */
                _input_buffer << charin;
                if (ASCII::IsControlCode(charin))
                {
                    PrintControlCode(charin);
                }
                else
                {
                    _output_buffer << charin;
                }
                break;
        }
        FlushOutput();
    }

    IOBuffer& Console::StdOut()
    {
        return _output_buffer;
    }

    void Console::PrintControlCode(char code)
    {
        if (_verbose_codes)
        {
            _output_buffer << '^' << ASCII::GetCodeCaretNotationChar(code);
        }
    }

    void Console::PrintCloseInput()
    {
        _output_buffer.InsertNewLine();
        _output_buffer << Options::ConsoleHeader;
    }

    void Console::FlushOutput()
    {
        /* we loop twice because it may happen that we wrap around the circ buffer
        so, technically, hacking an implementation limitation */
        for(auto i = 0; i < 2; ++i)
        {
            auto write_len = _output_buffer.GetBuffer().PeekUsed();
            if (write_len > 0)
            {
                _serial.write(_output_buffer.GetBuffer().GetContiguousArray(), write_len);
                _output_buffer.GetBuffer().Discard(write_len);
            }
        }
    }

    void Console::ParseInputExecuteCommand()
    {
        bool (*space_test)(char) = [](char testchar) -> bool
        {
            return testchar == ' ' or '\t' == testchar;
        };
        bool (*not_space)(char) = [](char testchar) -> bool
        {
            return not (testchar == ' ' or '\t' == testchar);
        };

        _output_buffer.InsertNewLine();

        auto start = _input_buffer.GetBuffer().GetCursor();
        auto space_marker = start;
        space_marker.Next(space_test);

        /* Attempt to process normal cli commands */
        if (IsStringEqual(start, space_marker, "cli"))
        {
            /* skip found space */
            space_marker.Next(not_space);
            auto start = space_marker;
            space_marker.Next(space_test);
            for (const auto& opt : _options)
            {
                if (IsStringEqual(start, space_marker, opt.name))
                {
                    auto ret = ((*this).*(opt.run))(_input_buffer, _output_buffer);
                    return;
                }
            }
            auto ret = Help(_input_buffer, _output_buffer);
            return;
        }

        /* else attempt to find registered commands */
        for(auto i = 0; ; ++i)
        {
            auto cmd = builtin_commands[i];
            /* Reached the end of the list */
            if (not cmd.name)
            {
                _output_buffer << "Command '";
                // todo: fixme!
                // _output_buffer.GetBuffer().Push(_input_buffer, space_marker.LowerDistance());
                _output_buffer << "' is not registered.";
                return;
            }
            if (IsStringEqual(start, space_marker, cmd.name))
            {
                auto ret =  cmd.run(_input_buffer, _output_buffer);
                return;
            }
        }

        /* else... fuck. */
        _output_buffer << "fuck!";
    }

    types::ReturnCode Console::Help(IOBuffer& input, IOBuffer& output)
    {
        auto i = 0;
        for (const auto& opt : _options)
        {
            ++i;
            _output_buffer.InsertString(opt.name, i < types::Length(_options));
        }
        return types::ReturnCode::Success;
    }

    types::ReturnCode Console::VerboseCodes(IOBuffer& input, IOBuffer& output)
    {
        /* toogle verbose */
        _verbose_codes ^= true;
        _output_buffer.InsertNewLine();
        _output_buffer << "Verbose codes are now ";
        if (_verbose_codes)
        {
            _output_buffer << "enabled.";
        }
        else
        {
            _output_buffer << "disabled.";
        }
        return types::ReturnCode::Success;
    }

} /* namespace Console */
