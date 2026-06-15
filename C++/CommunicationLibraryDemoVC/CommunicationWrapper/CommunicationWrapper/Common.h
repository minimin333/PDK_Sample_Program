#pragma once

using namespace System;
using namespace System::Reflection;
using namespace System::IO;
using namespace ODT::PowerPmacComLib;
//using namespace ODT::Common;
//using namespace ODT::PMACGlobal;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace ODT::MaskForm;

#using<System.Windows.Forms.dll>

#define DllExport __declspec(dllexport)

#define SUCCESS 0
#define INVALIDLICENSE 1
#define FAILEDTOCONNECT 2
#define DEVICENOTCONNECT 3
#define ERRORSEVENTSTART 4
#define ERRORSEVENTSTOP 5
#define UNSOLICITEDTIMEOUT 6
#define PORTNOTAVAILABLE 7
#define BUFFERNOTDEFINED 8
#define SOMEEXCEPTION 9
#define EVENTNOTSTARTED 10

#define GREATERTHANZERO 1			
#define LESSTHANEQUALTOZERO 2
