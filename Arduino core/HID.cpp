

/* Copyright (c) 2011, Peter Barrett  
**  
** Permission to use, copy, modify, and/or distribute this software for  
** any purpose with or without fee is hereby granted, provided that the  
** above copyright notice and this permission notice appear in all copies.  
** 
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
** SOFTWARE.  
*/

#include "USBAPI.h"

#if defined(USBCON)
#ifdef HID_ENABLED

//#define RAWHID_ENABLED
#define JOYHID_ENABLED
//	Singletons for mouse and keyboard

Joystick_ Joystick;
//================================================================================
//================================================================================

//	HID report descriptor

#define LSB(_x) ((_x) & 0xFF)
#define MSB(_x) ((_x) >> 8)

#define RAWHID_USAGE_PAGE	0xFFC0
#define RAWHID_USAGE		0x0C00
#define RAWHID_TX_SIZE 64
#define RAWHID_RX_SIZE 64

extern const u8 _hidReportDescriptor[] PROGMEM;
const u8 _hidReportDescriptor[] = {
	

#ifdef JOYHID_ENABLED
// 32 buttons (and a throttle - just in case the game doesn't recognise a joystick with no analog axis)

	0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
	0x09, 0x04,			// USAGE (Joystick)
	0xa1, 0x01,			// COLLECTION (Application)
		0x85, 0x03,			// REPORT_ID (3)  (This is important when HID_SendReport() is called)

		//Buttons:
		0x05, 0x09,			// USAGE_PAGE (Button)
		0x19, 0x01,			// USAGE_MINIMUM (Button 1)
		0x29, 0x20,			// USAGE_MAXIMUM (Button 32)
		0x15, 0x00,			// LOGICAL_MINIMUM (0)
		0x25, 0x01,			// LOGICAL_MAXIMUM (1)
		0x75, 0x01,			// REPORT_SIZE (1)
		0x95, 0x20,			// REPORT_COUNT (32)
		0x55, 0x00,			// UNIT_EXPONENT (0)
		0x65, 0x00,			// UNIT (None)
		0x81, 0x02,			// INPUT (Data,Var,Abs)

		// 8 bit Throttle and Steering
		0x05, 0x02,			// USAGE_PAGE (Simulation Controls)

		0x15, 0x00,			// LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,		// LOGICAL_MAXIMUM (255)
		0xA1, 0x00,			// COLLECTION (Physical)
			0x09, 0xBB,			// USAGE (Throttle)
			0x09, 0xBA,			// USAGE (Steering)
			0x75, 0x08,			// REPORT_SIZE (8)
			0x95, 0x02,			// REPORT_COUNT (2)
			0x81, 0x02,			// INPUT (Data,Var,Abs)

		0xc0,				// END_COLLECTION
		// Two Hat switches

		0x05, 0x01,			// USAGE_PAGE (Generic Desktop)

		0x09, 0x39,			// USAGE (Hat switch)
		0x15, 0x00,			// LOGICAL_MINIMUM (0)
		0x25, 0x07,			// LOGICAL_MAXIMUM (7)
		0x35, 0x00,			// PHYSICAL_MINIMUM (0)
		0x46, 0x3B, 0x01,		// PHYSICAL_MAXIMUM (315)
		0x65, 0x14,			// UNIT (Eng Rot:Angular Pos)
		0x75, 0x04,			// REPORT_SIZE (4)
		0x95, 0x01,			// REPORT_COUNT (1)
		0x81, 0x02,			// INPUT (Data,Var,Abs)

		0x09, 0x39,			// USAGE (Hat switch)
		0x15, 0x00,			// LOGICAL_MINIMUM (0)
		0x25, 0x07,			// LOGICAL_MAXIMUM (7)
		0x35, 0x00,			// PHYSICAL_MINIMUM (0)
		0x46, 0x3B, 0x01,		// PHYSICAL_MAXIMUM (315)
		0x65, 0x14,			// UNIT (Eng Rot:Angular Pos)
		0x75, 0x04,			// REPORT_SIZE (4)
		0x95, 0x01,			// REPORT_COUNT (1)
		0x81, 0x02,			// INPUT (Data,Var,Abs)

		0x15, 0x00,			// LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,		// LOGICAL_MAXIMUM (255)
		0x75, 0x08,			// REPORT_SIZE (8)

		0x09, 0x01,			// USAGE (Pointer)
		0xA1, 0x00,			// COLLECTION (Physical)
			0x09, 0x30,		// USAGE (x)
			0x09, 0x31,		// USAGE (y)
			0x09, 0x32,		// USAGE (z)
			0x09, 0x33,		// USAGE (rx)
			0x09, 0x34,		// USAGE (ry)
			0x09, 0x35,		// USAGE (rz)
			0x95, 0x06,		// REPORT_COUNT (2)
			0x81, 0x02,		// INPUT (Data,Var,Abs)
		0xc0,				// END_COLLECTION

	0xc0					// END_COLLECTION




#endif
};

extern const HIDDescriptor _hidInterface PROGMEM;
const HIDDescriptor _hidInterface =
{
	D_INTERFACE(HID_INTERFACE,1,3,0,0),
	D_HIDREPORT(sizeof(_hidReportDescriptor)),
	D_ENDPOINT(USB_ENDPOINT_IN (HID_ENDPOINT_INT),USB_ENDPOINT_TYPE_INTERRUPT,0x40,0x01)
};

//================================================================================
//================================================================================
//	Driver

u8 _hid_protocol = 1;
u8 _hid_idle = 1;

#define WEAK __attribute__ ((weak))

int WEAK HID_GetInterface(u8* interfaceNum)
{
	interfaceNum[0] += 1;	// uses 1
	return USB_SendControl(TRANSFER_PGM,&_hidInterface,sizeof(_hidInterface));
}

int WEAK HID_GetDescriptor(int /* i */)
{
	return USB_SendControl(TRANSFER_PGM,_hidReportDescriptor,sizeof(_hidReportDescriptor));
}

void WEAK HID_SendReport(u8 id, const void* data, int len)
{
	USB_Send(HID_TX, &id, 1);
	USB_Send(HID_TX | TRANSFER_RELEASE,data,len);
}

bool WEAK HID_Setup(Setup& setup)
{
	u8 r = setup.bRequest;
	u8 requestType = setup.bmRequestType;
	if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType)
	{
		if (HID_GET_REPORT == r)
		{
			//HID_GetReport();
			return true;
		}
		if (HID_GET_PROTOCOL == r)
		{
			//Send8(_hid_protocol);	// TODO
			return true;
		}
	}
	
	if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
	{
		if (HID_SET_PROTOCOL == r)
		{
			_hid_protocol = setup.wValueL;
			return true;
		}

		if (HID_SET_IDLE == r)
		{
			_hid_idle = setup.wValueL;
			return true;
		}
	}
	return false;
}
//================================================================================
//================================================================================
// Joystick
//  Usage: Joystick.move(x, y, throttle, buttons)
//  x & y forward/left = 0, centre = 127, back/right = 255
//  throttle max = 0, min = 255
//  8 buttons packed into 1 byte

Joystick_::Joystick_()
{
}

#define joyBytes 13 		// should be equivalent to sizeof(JoyState_t)

void Joystick_::setState(JoyState_t *joySt)
{
	uint8_t data[joyBytes];
	uint32_t buttonTmp;
	buttonTmp = joySt->buttons;

	data[0] = buttonTmp & 0xFF;		// Break 32 bit button-state out into 4 bytes, to send over USB
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[2] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[3] = buttonTmp & 0xFF;

	data[4] = joySt->throttleLeft;		// Throttle left
	data[5] = joySt->rudder;		// Steering

	data[6] = (joySt->hatSw2 << 4) | joySt->hatSw1;		// Pack hat-switch states into a single byte

	data[7] = joySt->xAxis;		// X axis
	data[8] = joySt->yAxis;		// Y axis
	data[9] = joySt->throttleRight;	// Right throttle
	data[10] = joySt->speedBrake;	// speed brake
	data[11] = joySt->flaps;	// Flaps
	data[12] = joySt->yaw;		// Yaw
	//HID_SendReport(Report number, array of values in same order as HID descriptor, length)
	HID_SendReport(3, data, joyBytes);
	// The joystick is specified as using report 3 in the descriptor. That's where the "3" comes from
}

#endif

#endif /* if defined(USBCON) */

