/*
||
|| @file Potentiometer.cpp
|| @version 1.2
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making/using potentiometers
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#include "Potentiometer.h"

Potentiometer::Potentiometer(byte potPin){
	pin=potPin;
	setSectors(6);
}

Potentiometer::Potentiometer(byte potPin, uint16_t sectors){
	pin=potPin;
	setSectors(sectors);
}

uint16_t Potentiometer::getValue(){
	return analogRead(pin);
}

uint16_t Potentiometer::getSector(){
	return analogRead(pin)/(1024/sectors);
}

void Potentiometer::setSectors(uint16_t newSectors){
	if (newSectors<1024 && newSectors>0){
		sectors=newSectors;
	}else if (newSectors<0){
		sectors=0;
	}else{
		sectors=1023;
	}
}

/*
|| @changelog
|| | 1.0 2009-04-13 - Alexander Brevig : Initial Release
|| #
*/