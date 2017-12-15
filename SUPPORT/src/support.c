/*
Vic's IO Board V1.0 Copyright (C) 2017 Vidas Simkus

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "support.h"
#include <string.h>
#include <stdlib.h> //ltoa

inline unsigned char rotl_b(unsigned char _byte, unsigned char _num)
{
	while (_num > 0)
	{
		ROTL_B(_byte);
		_num = _num - 1;
	}

	return _byte;
}

inline unsigned char rotr_b(unsigned char _byte, unsigned char _num)
{
	while (_num > 0)
	{
		ROTR_B(_byte);
		_num = _num - 1;
	}

	return _byte;
}

SINT find_in_string(const char * _str, UCHAR _char, UINT _start_idx, UINT _max_length)
{
	UINT idx = 0;
	UCHAR c = 0;
	SINT ret = -1;

	for (idx = _start_idx; idx < _max_length; idx++)
	{
		c = _str[idx];

		if (c == 0)
		{
			/*
			 * Found the end of the string without finding the char.
			 */
			break;
		}

		if (c == _char)
		{
			ret = idx;
			break;
		}
	}

	return ret;
}

/*
 * Note:  I borrowed this from:
 * http://www.microchip.com/forums/tm.aspx?m=69532#69589
 */
void ftoa (char *buf, double fval, int cField, int cPlaces)
{
	//This routine correctly formats a floating point number into it's ASCII representation.
	//The routine will insert leading spaces as necessary to satisfy the requirements of the
	//specified formatting.
	//
	//eg:
	//ftoa(buf,12.34567,5,3);
	//Ask for 5 digit field including '.',  and 3 digits after.
	//After the routine, buf will contain: '12.34'.
	//
	//This allows for easier integration of displays onto the VFD.
	//

	long lTemp;
	long scale;
	int i;
	char dbuf[8];	//Temporary - used and overwritten in routine
	char pbuf[8];	//Temporary - used and overwritten in routine
	char fbuf[16];	//The main 'build' buffer.  The output string is built here and copied into the *buf buffer at the end.

	char cDigits;
	size_t szTemp;

	if (cPlaces)
	{
		cDigits = cField - cPlaces - 1;	//FP formatting.  Includes '.' and decimal part
	}
	else
	{
		cDigits = cField;			//Special case for integers
	}

	//EXAMPLE USED: fval=123.456789, fp(5,5) has been set
	memset(fbuf, 0, sizeof (fbuf));	//Clear the buffer
	scale = 1;					//Set default scale value

	lTemp = (long) fval;			//convert double to long	- ltemp=123
	ltoa(dbuf, lTemp, 10);			//convert long to ascii in dbuf
	szTemp = strlen(dbuf);		//How many digits does the number use?


	if ((char) szTemp < cDigits)	//Do we need leading whitespaces?
	{
		for (i = 0; i < cDigits - (int) szTemp + 0; i++)
		{
			pbuf[i] = ' ';		//Insert the reqd number of spaces into the buffer 'pbuf'
		}

		if ((fval < 0) && (fval>-1) )  //Special case for -0.1 (etc) where 'digit' is = 0, but fval is -ve.
		{
			pbuf[i - 1] = '-';		//Add -ve symbol to the main buffer
		}
		pbuf[i] = 0;				//Null terminate the spaces so strcat can find it
		//No need to add '1' - the for loop does this for us!
		strcat(fbuf, pbuf);		//Add the spaces (and sign?) into the main buffer
	}
	else
	{
		if ((fval < 0) && (fval>-1) )  //Special case for -0.1 (etc) where 'digit' is = 0, but fval is -ve.
		{
			fbuf[0] = '-';				//Add -ve symbol to the main buffer
			fbuf[1] = 0;				//Null terminate
		}
	}
	strcat(fbuf, dbuf);			//Add the numerical part to the main buffer (after '-' and spaces if reqd)

	if (cPlaces)				//Do we need the decimal part?
	{
		strcat(fbuf, ".");		//add '.' to end of main buffer


		//Now scale up the decimal part...

		//for(i=1; i<=giPlaces; i++)
		//{
		//	scale=scale*10;			//work out the scale according to 'giPlaces'
		//}							//eg with fp(5,5) scale=100000
		//fval=fval*scale;			//scale up decimal part - fval=45678.9
		switch (cPlaces)
		{
			case 1:	scale = 10;
				break;
			case 2:	scale = 100;
				break;
			case 3: scale = 1000;
				break;
			case 4:	scale = 10000;
				break;
			case 5: scale = 100000;
				break;
		}

		fval = fval - (int) fval; 		//Remove integer part... fval=0.456789

		if (fval >= 0)		// Do not lose "0" in fractional part (i.e. 0.0456)
			fval += 1;
		else
			fval -= 1;

		fval *= scale;

		if (fval >= 0)
			fval += 0.5f;		//  For positive round number up
		else
			fval -= 0.5f;		// For negative round number down

		lTemp = (long) fval;			//convert to long integer - ltemp=45678

		if (lTemp < 0) lTemp = -lTemp;	//No negative fractions!!! (prevents -1.-234 etc)
		ltoa(dbuf, lTemp, 10);			//copy into dbuf - dbuf='45678'
		strcat(fbuf, &dbuf[1]);			//add the numerical part to the main buffer
	}

	strncpy(buf, fbuf, (size_t) cField);		//copy the final result to the output buffer
	buf[cField] = 0;					//Null terminate buf
}

inline UINT checksum( UINT const data[], UINT nWords )
{
	ULONG sum = 0;

	/*
	 * IP headers always contain an even number of bytes.
	 */
	while ( nWords-- > 0 )
	{
		sum += *( data++ );
	}

	/*
	 * Use carries to compute 1's complement sum.
	 */
	sum = ( sum >> 16 ) + ( sum & 0xFFFF );
	sum += sum >> 16;

	/*
	 * Return the inverted 16-bit result.
	 */
	return ((UINT ) ~sum );
}

