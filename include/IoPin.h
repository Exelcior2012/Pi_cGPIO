
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class IoPin
{
public:

	enum class Direction
	{
		Input,
		Output
	};

	IoPin() = delete;
	IoPin(const IoPin&) = delete;
	IoPin& operator=(const IoPin&) = delete;
	
	IoPin(int InPinId, Direction InDirection)
		: PinId(InPinId)
		, CurrentDirection(InDirection)
		, State(false)
	{
		ValidatePinId();
		
		if(IsValid())
		{
			Export();
			SetDirection(CurrentDirection, true);
		}
	}
	
	~IoPin()
	{
		if(IsValid())
		{
			UnExport();
		}
	}
	
	bool SetDirection(Direction InDirection, bool bInForce = false)
	{
		if(!IsValid())
		{
			return false;
		}
		
		// Only write the direction if we really need to
		if(InDirection != CurrentDirection || bInForce)
		{
			string DirectionLocation = GpioBase + to_string(PinId) + GpioDirection;
			return WriteTo(DirectionLocation, DirectionString(InDirection));
		}
		return true;
	}
	
	bool On()
	{
		if(!IsValid())
		{
			return false;
		}
		
		string ValueLocation = GpioBase + to_string(PinId) + GpioValue;
		if(WriteTo(ValueLocation, "1"))
		{
			State = true;
			return true;
		}
		
		return false;
	}
	
	bool Off()
	{
		if(!IsValid())
		{
			return false;
		}
		
		string ValueLocation = GpioBase + to_string(PinId) + GpioValue;
		if(WriteTo(ValueLocation, "0"))
		{
			State = false;
			return true;
		}
		
		return false;
	}
	
	int Get()
	{
		int Result = -1;
	
		if(!IsValid())
		{
			return Result;
		}
		
		string ValueLocation = GpioBase + to_string(PinId) + GpioValue;
		ifstream File(ValueLocation.c_str());
		if(File.bad())
		{
			cout << "Failed to open " << ValueLocation << endl;
			return Result;
		}
		
		string TempValue;
		File >> TempValue;
		
		if(TempValue != "0")
		{
			Result = 1;
		}
		else
		{
			Result = 0;
		}
		
		return Result;
	}
	
	int GetId()
	{
		return PinId;
	}

	bool IsValid()
	{
		return PinId != INVALID_PIN;
	}

private:

	bool Export()
	{
		return WriteTo(ExportLocation, to_string(PinId));
	}
	
	bool UnExport()
	{
		return WriteTo(UnExportLocation, to_string(PinId));
	}

	string DirectionString(Direction InDirection)
	{
		switch(InDirection)
		{
			case Direction::Input: 
				return "in";
				break;
			case Direction::Output:
				return "out";
				break;
			default:
				break;
		}
		
		return "";
	}

	bool WriteTo(string InLocation, string InValue)
	{
		ofstream File(InLocation.c_str());
		if(File.bad())
		{
			cout << "Failed to write file [" << InLocation << endl;
			return false;
		}
		File << InValue;
		File.close();
		return true;
	}
	
	void ValidatePinId()
	{
		for(int i = 0; i < NumValidPinIds; ++i)
		{
			if(PinId == ValidIds[i])
			{
				return;
			}
		}
		PinId = INVALID_PIN;
	}
	
	static const string ExportLocation;
	static const string UnExportLocation;
	static const string GpioBase;
	static const string GpioDirection;
	static const string GpioValue;
	static const int NumValidPinIds = 22;
	static const int ValidIds[NumValidPinIds];
	static const int INVALID_PIN = -1;

	int PinId;
	Direction CurrentDirection;
	bool State;

};

const string IoPin::ExportLocation = "/sys/class/gpio/export";
const string IoPin::UnExportLocation = "/sys/class/gpio/unexport";
const string IoPin::GpioBase = "/sys/class/gpio/gpio";
const string IoPin::GpioDirection = "/direction";
const string IoPin::GpioValue = "/value";
const int IoPin::ValidIds[IoPin::NumValidPinIds] = {4,17,18,27,22,23,24,10,9,25,11,8,7,5,6,12,13,19,16,26,20,21};
