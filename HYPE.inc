/* * * * * * * * * * * * *( HYPE 1.0 )* * * * * * * * * * * * *\
 *                                                            *
 *        (C) Copyright 2022 by zHypezinXzL (R)               *
 *                                                            *
 * @country  : Brasil                                         *
 * @author   : zHypezinXzL                                    *
 * @github   : https://github.com/zHypezinXzL                 *
 * @date     : 7th Setember 2022 (Independence Brasil)        *
 *                                                            *
 * Time to Drop DOF2 and Dini and start using HYPE            *
 *                                                            *
\* * * * * * * * * * * * *( HYPE 1.0 )* * * * * * * * * * * * */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <a_samp>             /*  Dowload:  https://github.com/zHypezinXzL/include-a_samp.git  */
#include <string>             /*  Dowload:  https://github.com/zHypezinXzL/include-string.git  */
#include <float>              /*  Dowload:  https://github.com/zHypezinXzL/include-float.git   */
#include <file>               /*  Dowload:  https://github.com/zHypezinXzL/include-file.git    */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



/* * * * * * * * * * * * * * * */
#if defined _hype_included
    #endinput
#endif
#define _hype_included
#pragma library hype
#if defined MAX_STRING  // * * * * * * * *  :  Var Exists
    #define HYPE_MAX_STRING MAX_STRING
#else // * * * * * * * * * * * * * * * * *  :  Var Not Exists
    #define HYPE_MAX_STRING 255
#endif
/* * * * * * * * * * * * * * * */



// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FCreate(const filename[])
{
    if(HYPE_FExists(filename)) return 0;

	new File:fhnd;
	fhnd = fopen(filename, io_write);
	if(fhnd)
    {
		return fclose(fhnd);
	}
	return 0;
}
stock HYPE_FRemove(const filename[])
{
    if(!HYPE_FExists(filename)) return 0;

	return fremove(filename);
}
stock HYPE_FExists(const filename[])
{
	return fexist(filename);
}
stock HYPE_FRename(const oldname[], const newname[])
{
	if(HYPE_FExists(newname)) return 0;

    new File:fhnd; fhnd = fopen(newname, io_write);
    if(fhnd)
    {
        HYPE_FCopyText(oldname, newname);
        fclose(fhnd);
    }
	return fremove(oldname);
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FSetString(const filename[], const key[], const value[])
{
    if(!HYPE_FExists(filename)) return 0;

    if(HYPE_FIsSet(filename, key)) HYPE_FUnSet(filename, key);
	new key_size = strlen(key); key_size = strlen(value);
	if(key_size == 0 || key_size + key_size + 2 > HYPE_MAX_STRING) return false;
	new File:fohnd, File:fwhnd, tmpres[HYPE_MAX_STRING], bool:wasset = false;
	format(tmpres, sizeof tmpres, "%s.part", filename);
	fremove(tmpres);
	fohnd = fopen(filename, io_read);
	if(!fohnd) return false;
	fwhnd = fopen(tmpres, io_write);
	if(!fwhnd)
    {
		fclose(fohnd);
		return false;
	}
	while(fread(fohnd, tmpres))
    {
		if(!wasset && tmpres[key_size] == '=' && !strcmp(tmpres, key, true, key_size))
        {
 			format(tmpres, sizeof tmpres, "%s=%s", key, value);
			wasset = true;
		}
        else
        {
            HYPE_FNewLine(tmpres);
        }
		fwrite(fwhnd, tmpres);
		fwrite(fwhnd, "\r\n");
	}
	if(!wasset)
    {
		format(tmpres, sizeof tmpres, "%s=%s", key, value);
		fwrite(fwhnd, tmpres);
		fwrite(fwhnd, "\r\n");
	}
	fclose(fohnd);
	fclose(fwhnd);
	format(tmpres, sizeof tmpres, "%s.part", filename);
	if(HYPE_FCopyText(tmpres, filename))
    {
		return fremove(tmpres);
	}
	return false;
}
stock HYPE_FGetString(const filename[], const key[])
{
	new tmpres[HYPE_MAX_STRING];
	new key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_STRING) return tmpres;
	new File:fohnd;
	fohnd = fopen(filename, io_read);
	if(!fohnd) return tmpres;
	while(fread(fohnd,tmpres))
    {
		if(tmpres[key_size] == '=' && !strcmp(tmpres, key, true, key_size))
        {
			HYPE_FNewLine(tmpres);
			strmid(tmpres, tmpres, key_size + 1, strlen(tmpres), HYPE_MAX_STRING);
			fclose(fohnd);
			return tmpres;
		}
	}
	fclose(fohnd);
	return tmpres;
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FSetInt(const filename[], const key[], value)
{
    if(!HYPE_FExists(filename)) return 0;

    new StrValue[HYPE_MAX_STRING];
    if(HYPE_FIsSet(filename, key)) HYPE_FUnSet(filename, key);
    format(StrValue, sizeof StrValue, "%d", value);
    return HYPE_FSetString(filename, key, StrValue);
}
stock HYPE_FGetInt(const filename[], const key[])
{
    if(!HYPE_FExists(filename)) return 0;

    return strval(HYPE_FGetString(filename, key));
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FSetFloat(const filename[], const key[], Float:value)
{
    if(!HYPE_FExists(filename)) return 0;

    new StrValue[HYPE_MAX_STRING];
    if(HYPE_FIsSet(filename, key)) HYPE_FUnSet(filename, key); 
    format(StrValue, sizeof StrValue, "%f", value);
    return HYPE_FSetString(filename, key, StrValue);
}
stock Float:HYPE_FGetFloat(const filename[], const key[])
{
    return floatstr(HYPE_FGetString(filename, key));
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FSetBool(const filename[], const key[], bool:value)
{
    if(!HYPE_FExists(filename)) return 0;

    if(HYPE_FIsSet(filename, key)) HYPE_FUnSet(filename, key);
	if(value == true) return HYPE_FSetString(filename, key, "true");
    else return HYPE_FSetString(filename, key, "false");
}
stock bool:HYPE_FGetBool(const filename[], const key[])
{
	new bool[50]; 
	format(bool, sizeof bool, "%s", HYPE_FGetString(filename, key));
    if(strcmp(bool, "true") == 0) return true;
	else return false;
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FIsSet(const filename[], const key[])
{
    if(!HYPE_FExists(filename)) return 1;

	new key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_STRING) return 0;
	new File:fohnd, tmpres[HYPE_MAX_STRING];
	fohnd = fopen(filename, io_read);
	if(!fohnd) return 0;
	while(fread(fohnd, tmpres))
    {
		if(tmpres[key_size] == '=' && !strcmp(tmpres, key, true, key_size))
        {
			fclose(fohnd);
			return 1;
		}
	}
	fclose(fohnd);
	return 0;
}
stock HYPE_FUnSet(const filename[], const key[])
{
    if(!HYPE_FExists(filename)) return 0;

	new key_size = strlen(key);
	if(key_size == 0 || key_size + 2 > HYPE_MAX_STRING) return 0;
	new File:fohnd, File:fwhnd;
	new tmpres[HYPE_MAX_STRING];
	format(tmpres, HYPE_MAX_STRING, "%s.part", filename);
	fremove(tmpres);
	fohnd = fopen(filename, io_read);
	if(!fohnd) return 0;
	fwhnd = fopen(tmpres, io_write);
	if(!fwhnd)
    {
		fclose(fohnd);
		return false;
	}
	while(fread(fohnd,tmpres))
    {
		if(tmpres[key_size] == '=' && !strcmp(tmpres, key, true, key_size))
        {
        }
        else
        {
			HYPE_FNewLine(tmpres);
			fwrite(fwhnd, tmpres);
			fwrite(fwhnd, "\r\n");
		}
	}
	fclose(fohnd);
	fclose(fwhnd);
	format(tmpres, HYPE_MAX_STRING, "%s.part", filename);
	if(HYPE_FCopyText(tmpres, filename))
    {
		return fremove(tmpres);
	}
	return 0;
}
// ----------- ------------ ---------- ------------ ---------- ---------
stock HYPE_FNewLine(code[])  // const return error / no const return sucess
{
	new size = strlen(code);
	if(code[0] == 0) return ;
	if((code[size-1] == '\n') || (code[size - 1] == '\r'))
    {
		code[size - 1] = 0;
		if(code[0] == 0) return ;
		else if((code[size - 2] == '\n') || (code[size - 2] == '\r'))
        {
            code[size - 2] = 0;
            if(code[0] == 0) return ;
        }
	}
}
stock HYPE_FCopyText(const oldname[], const newname[])
{
	new File:ohnd, File:nhnd;
	if(!fexist(oldname)) return 0;
	ohnd = fopen(oldname, io_read);
	if(!ohnd) return 0;
	nhnd = fopen(newname, io_write);
	if(!nhnd)
    {
		return fclose(ohnd);
	}
	new tmpres[HYPE_MAX_STRING];
	while(fread(ohnd, tmpres))
    {
		HYPE_FNewLine(tmpres);
		format(tmpres, sizeof tmpres, "%s\r\n", tmpres);
		fwrite(nhnd, tmpres);
	}
	fclose(ohnd);
	fclose(nhnd);
	return 1;
}
/* ----------- ------------ ---------- ------------ ---------- ---------

native <--=[C]=-->
native HYPE_FCreate(const file[]);                                          // FC

native <--=[E]=-->
native HYPE_FExists(const file[]);                                          // FE

native <--=[G]=-->
native HYPE_FGetBool(const file[], const key[]);                            // FGB
native HYPE_FGetFloat(const file[], const key[]);                           // FGF
native HYPE_FGetInt(const file[], const key[]);                             // FGI
native HYPE_FGetString(const file[], const key[]);                          // FGS

native <--=[I]=-->
native HYPE_FIsSet(const file[], cosnt key[]);                              // FI

native <--=[S]=-->
native HYPE_FSetBool(const file[], const key[], bool:value);                // FSB
native HYPE_FSetFloat(const file[], const key[], Float:value);              // FSF
native HYPE_FSetInt(const file[], const key[], value);                      // FSI
native HYPE_FSetString(const file[], const key[], const value[]);           // FSS

native <--=[R]=-->
native HYPE_FRemove(const file[]);                                          // FRM
native HYPE_FRename(const file[]);                                          // FRN

native <--=[U]=-->
native HYPE_FUnSet(const file[], const key[]);                              // FU

// ----------- ------------ ---------- ------------ ---------- ---------                                                                                                                                                                   */
