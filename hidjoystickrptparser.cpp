#include "hidjoystickrptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) : joyEvents(evt),
                                                                  oldHat(0xDE),
                                                                  oldButtons(0)
{
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                oldPad[i] = 0xD;
}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
        bool match = true;

        // Checking if there are changes in report since the method was last called
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                if (buf[i] != oldPad[i])
                {
                        match = false;
                        break;
                }

        // Calling Game Pad event handler
        if (!match && joyEvents)
        {
                joyEvents->OnGamePadChanged((const GamePadEventData *)buf);

                for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                        oldPad[i] = buf[i];
        }

        uint8_t hat = (buf[5] & 0xF);

        // Calling Hat Switch event handler
        if (hat != oldHat && joyEvents)
        {
                joyEvents->OnHatSwitch(hat);
                oldHat = hat;
        }

        uint16_t buttons = (0x0000 | buf[6]);
        buttons <<= 4;
        buttons |= (buf[5] >> 4);
        uint16_t changes = (buttons ^ oldButtons);

        // Calling Button Event Handler for every button changed
        if (changes)
        {
                for (uint8_t i = 0; i < 0x0C; i++)
                {
                        uint16_t mask = (0x0001 << i);

                        if (((mask & changes) > 0) && joyEvents)
                        {
                                if ((buttons & mask) > 0)
                                        joyEvents->OnButtonDn(i + 1);
                                else
                                        joyEvents->OnButtonUp(i + 1);
                        }
                }
                oldButtons = buttons;
        }
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{

        X = evt->X;
        Y = evt->Y;
        Z1 = evt->Z1;
        Z2 = evt->Z2;
        Rz = evt->Rz;

        /*
        Serial.print("X1: ");
        PrintHex<uint8_t>(evt->X, 0x80);
        Serial.print("\tY1: ");
        PrintHex<uint8_t>(evt->Y, 0x80);
        Serial.print("\tX2: ");
        PrintHex<uint8_t>(evt->Z1, 0x80);
        Serial.print("\tY2: ");
        PrintHex<uint8_t>(evt->Z2, 0x80);
        Serial.print("\tRz: ");
        PrintHex<uint8_t>(evt->Rz, 0x80);
        Serial.println("");
        */
}

void JoystickEvents::OnHatSwitch(uint8_t hat)
{ /*
        Serial.print("Hat Switch: ");
        // PrintHex<uint8_t > (hat, 0x80);
        Serial.println(hat);
        Serial.println("");
        */

        if (hat == 0)
        {
                valueRead = 0;
        }
        else if (hat == 4)
        {
                valueRead = 4;
        }
        else if (hat == 6)
        {
                valueRead = 6;
        }
        else if (hat == 2)
        {
                valueRead = 2;
        }
        else
        {
                valueRead = 5;
        }
}

void JoystickEvents::OnButtonUp(uint8_t but_id)
{ /*
        Serial.print("Up: ");
        Serial.println(but_id, DEC);
        */

        if (but_id == 2)
        {
                buttonPress = 0;
        }
        else if (but_id == 3)
        {
                buttonPress = 0;
        }
        else if (but_id == 1)
        {
                buttonPress = 0;
        }
        else if (but_id == 4)
        {
                buttonPress = 0;
        }
        else if (but_id == 7)
        {
                buttonPress = 0;
        }
        else if (but_id == 5)
        {
                buttonPress = 0;
        }
        else if (but_id == 8)
        {
                buttonPress = 0;
        }
        else if (but_id == 6)
        {
                buttonPress = 0;
        }
}

void JoystickEvents::OnButtonDn(uint8_t but_id)
{ /*
        Serial.print("Dn: ");
        Serial.println(but_id, DEC);
        */
        if (but_id == 2)
        {
                buttonPress = 2;
        }
        else if (but_id == 3)
        {
                buttonPress = 3;
        }
        else if (but_id == 1)
        {
                buttonPress = 1;
        }
        else if (but_id == 4)
        {
                buttonPress = 4;
        }
        else if (but_id == 7)
        {
                buttonPress = 7;
        }
        else if (but_id == 5)
        {
                buttonPress = 5;
        }
        else if (but_id == 8)
        {
                buttonPress = 8;
        }
        else if (but_id == 6)
        {
                buttonPress = 6;
        }
        else
        {
                buttonPress = 0;
        }
}
