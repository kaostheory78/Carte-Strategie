#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;


void affichage(int x, int y);

public ref class PortChat
{
private:
	static bool _continue;
	static SerialPort^ _serialPort;

public:
	static void Main()
	{
		String^ name;
		String^ message;
		StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		Thread^ readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));

		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort();

		// Allow the user to set the appropriate properties.
		_serialPort->PortName = "COM6";
		_serialPort->BaudRate = 115200;
		_serialPort->Parity = SetPortParity(_serialPort->Parity);
		_serialPort->DataBits = 8;
		_serialPort->StopBits = SetPortStopBits(_serialPort->StopBits);
		_serialPort->Handshake = SetPortHandshake(_serialPort->Handshake);

		// Set the read/write timeouts
		_serialPort->ReadTimeout = 500;
		_serialPort->WriteTimeout = 500;

		_serialPort->Open();
		_continue = true;
		readThread->Start();

		Console::WriteLine("Type QUIT to exit");

		while (_continue)
		{
			message = Console::ReadLine();

			if (stringComparer->Equals("quit", message))
			{
				_continue = false;
			}
			else
			{
				_serialPort->WriteLine(
					String::Format("<{0}>: {1}", "COM6", message));
			}
		}

		readThread->Join();
		_serialPort->Close();
	}

	static void Read()
	{
		while (_continue)
		{
			try
			{
				int x, y, i, j; 
				String^ coordonnee_x;
				String^ coordonnee_y;

				coordonnee_x = _serialPort->ReadLine();
				coordonnee_y = _serialPort->ReadLine();
				x = int::Parse(coordonnee_x);
				y = int::Parse(coordonnee_y);

				affichage(x, y);
			}
			catch (TimeoutException ^) {}
		}
	}

	static String^ SetPortName(String^ defaultPortName)
	{
		String^ portName;

		//Console::WriteLine("Available Ports:");
		//for each (String^ s in SerialPort::GetPortNames())
		//{
		//	Console::WriteLine("   {0}", s);
		//}

		//Console::Write("COM port({0}): ", defaultPortName);
		//portName = Console::ReadLine();

		//if (portName == "")
		//{
			portName = defaultPortName;
		//}
		return portName;
	}

	static Int32 SetPortBaudRate(Int32 defaultPortBaudRate)
	{
		String^ baudRate;

		Console::Write("Baud Rate({0}): ", defaultPortBaudRate);
		baudRate = Console::ReadLine();

		if (baudRate == "")
		{
			baudRate = defaultPortBaudRate.ToString();
		}

		return Int32::Parse(baudRate);
	}

	static Parity SetPortParity(Parity defaultPortParity)
	{
		String^ parity;
		parity = defaultPortParity.ToString();
		return (Parity)Enum::Parse(Parity::typeid, parity);
	}

	static StopBits SetPortStopBits(StopBits defaultPortStopBits)
	{
		String^ stopBits;
		stopBits = defaultPortStopBits.ToString();
		return (StopBits)Enum::Parse(StopBits::typeid, stopBits);
	}

	static Handshake SetPortHandshake(Handshake defaultPortHandshake)
	{
		String^ handshake;
		handshake = defaultPortHandshake.ToString();
		return (Handshake)Enum::Parse(Handshake::typeid, handshake);
	}
};

int main()
{
	PortChat::Main();
}

void affichage(int x, int y)
{
	int i, j;

	int _x = (int)(x / 100.);
	int _y = (int)(y / 100.);

	Console::Write(" _x : %d, _y : %d \n", _x, _y);
	Console::Write("X ");
	for (i = 0; i < 60; i++)
		Console::Write("~");
	Console::Write("\n");

	for (i = 0; i < 20; i++)
	{
		Console::Write(" |");
		for (j = 0; j < 30; j++)
		{
			if (_x == i  && _y == j)
				Console::Write("O ");
			else
				Console::Write("- ");
		}
		Console::Write("|");
		if (i == 10)
			Console::Write("X");
		Console::Write("\n\r");
	}

	Console::Write("X ");
	for (i = 0; i < 60; i++)
		Console::Write("~");
	Console::Write("\n");
}