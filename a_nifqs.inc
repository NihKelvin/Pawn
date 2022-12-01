/***********************************************
 *                                             *
 *                NiFQS - 1.0.0                *
 *                                             *
 *       @Author         : Kelvin/Nivlekzin    *
 *       @Date           : 12th Nov 2022       *
 *       @Time           : 00h 00m 00s PM      *
 *       @Update         : 20th Nov 2022       *
 *       @Version        : 1.0.0               *
 *                                             *
 *           Created By Nivlekzin :D           *
 *                                             *
 ***********************************************/


/*
*    Meaning of the acronym NiFQS
*    Ni = Nivlekzin
*    FQS = File Query System
*    Ni + FQS = NiFQS
*/

/*
native nifqs_FileExists(const filename[]);
native nifqs_RemoveFile(const filename[]);
native nifqs_CreateFile(const filename[]);
native nifqs_SetString(const filename[], const key[], const value[]);
native nifqs_GetString(const filename[], const key[]);
native nifqs_SetInt(const filename[], const key[], value);
native nifqs_GetInt(const filename[], const key[]);
native nifqs_SetBool(const filename[], const key[], bool:value);
native nifqs_GetBool(const filename[], const key[]);
native nifqs_SetFloat(const filename[], const key[], Float:value);
native nifqs_GetFloat(const filename[], const key[]);
native nifqs_UnSet(const filename[], const key[]);
native nifqs_IsSet(const filename[], const key[]);

Note: Always use <a_nifqs> below <a_samp>. Never use above or below another include to avoid bugs and errors.
*/


// |---------------- [DEFINES] ----------------| //
#if defined  _nifqs_included
    #endinput
#endif

#define _nifqs_included
#pragma library NiFQS

#if defined MAX_STRING
#define NiFQS_MAX_STRING MAX_STRING
#else
#define NiFQS_MAX_STRING 255
#endif


// |----------------- [STOCK] -----------------| //
stock nifqs_FileExists(const filename[])
{
    return fexist(filename);
}

stock nifqs_RemoveFile(const filename[])
{
    return fremove(filename);
}

stock nifqs_CreateFile(const filename[])
{
    if(fexist(filename))
        return false;
    
    new File:Archiver;
    Archiver = fopen(filename, io_write);

    if(Archiver)
    {
        fclose(Archiver);
        return true;
    }
    return false;
}

stock nifqs_SetString(const filename[], const key[], const value[])
{
    new key_length = strlen(key);
    new value_length = strlen(value);
    if(key_length == 0 || key_length + value_length + 2 > NiFQS_MAX_STRING)
        return false;

    new File:FirstFile, File:SecondFile;
    new StrFile[NiFQS_MAX_STRING];
    new bool:SetFile = false;

    format(StrFile, sizeof(StrFile), "%s.part", filename);
    fremove(StrFile);

    FirstFile = fopen(filename, io_read);
    if(!FirstFile)
        return false;
    
    SecondFile = fopen(StrFile, io_write);
    if(!SecondFile)
    {
        fclose(FirstFile);
        return false;
    }

    while(fread(FirstFile, StrFile))
    {
        if(!SetFile && StrFile[key_length] == '=' && !strcmp(StrFile, key, true, key_length))
        {
            format(StrFile, sizeof(StrFile), "%s=%s", key, value);
            SetFile = true;
        }
        else
        {
            NiFQS_StripNewLine(StrFile);
        }
        fwrite(SecondFile, StrFile);
        fwrite(SecondFile, "\r\n");
    }

    if(!SetFile)
    {
        format(StrFile, sizeof(StrFile), "%s=%s", key, value);
        fwrite(SecondFile, StrFile);
        fwrite(SecondFile, "\r\n");
    }

    fclose(FirstFile);
    fclose(SecondFile);

    format(StrFile, sizeof(StrFile), "%s.part", filename);
    if(NiFQS_fcopyTextFile(StrFile, filename))
    {
        return fremove(StrFile);
    }
    return false;
}

stock nifqs_SetInt(const filename[], const key[], value)
{
    new valuestring[NiFQS_MAX_STRING];
    format(valuestring, NiFQS_MAX_STRING, "%d", value);
    return nifqs_SetString(filename, key, valuestring);
}

stock nifqs_GetInt(const filename[], const key[])
{
    return strval(nifqs_GetString(filename, key));
}

stock nifqs_SetFloat(const filename[], const key[], Float:value)
{
    new valuestring[NiFQS_MAX_STRING];
    format(valuestring, NiFQS_MAX_STRING, "%f", value);
    return nifqs_SetString(filename, key, valuestring);
}

stock Float:nifqs_GetFloat(const filename[], const key[])
{
    return floatstr(nifqs_GetString(filename, key));
}

stock nifqs_GetBool(const filename[], const key[])
{
    return strval(nifqs_GetString(filename, key));
}

stock nifqs_SetBool(const filename[], const key[], bool:value)
{
    if(value == true)
    {
        return nifqs_SetString(filename, key, "1");
    }
    return nifqs_SetString(filename, key, "0");
}

stock nifqs_UnSet(const filename[], const key[])
{
    new key_length = strlen(key);
    if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
        return false;

    new File:FirstFile, File:SecondFile;
    new StrFile[NiFQS_MAX_STRING];

    format(StrFile, NiFQS_MAX_STRING, "%s.part", filename);
    fremove(StrFile);

    FirstFile = fopen(filename, io_read);
    if(!FirstFile)
        return false;

    SecondFile = fopen(StrFile, io_write);
    if(!SecondFile)
    {
        fclose(FirstFile);
        return false;
    }

    while(fread(FirstFile, StrFile))
    {
        if(StrFile[key_length] == '=' && !strcmp(StrFile, key, true, key_length))
        {
            
        }
        else
        {
            NiFQS_StripNewLine(StrFile);
            fwrite(SecondFile, StrFile);
            fwrite(SecondFile, "\r\n");
        }
    }

    fclose(FirstFile);
    fclose(SecondFile);

    format(StrFile, NiFQS_MAX_STRING, "%s.part", filename);
    if(NiFQS_fcopyTextFile(StrFile, filename))
    {
        return fremove(StrFile);
    }
    return false;
}

stock nifqs_GetString(const filename[], const key[])
{
    new StrFile[NiFQS_MAX_STRING];
	
	new key_length = strlen(key);
	if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
		return StrFile;
	
	new File:FirstFile;
	FirstFile = fopen(filename, io_read);
	if(!FirstFile)
		return StrFile;
	
	while(fread(FirstFile, StrFile))
	{
		if(StrFile[key_length] == '=' && !strcmp(StrFile, key, true, key_length))
		{
			NiFQS_StripNewLine(StrFile);
			strmid(StrFile, StrFile, key_length + 1, strlen(StrFile), NiFQS_MAX_STRING);
			fclose(FirstFile);
			return StrFile;
		}
	}

	fclose(FirstFile);
	return StrFile;
}

stock nifqs_IsSet(const filename[], const key[])
{
    new key_length = strlen(key);
	if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
		return false;
	
	new File:FirstFile;
	FirstFile = fopen(filename, io_read);
	if(!FirstFile) return false;
	
	new StrFile[NiFQS_MAX_STRING];
	while (fread(FirstFile, StrFile))
    {
		if (StrFile[key_length] == '=' && !strcmp(StrFile, key, true, key_length))
        {
	        fclose(FirstFile);
			return true;
		}
	}

	fclose(FirstFile);
	return false;
}

stock NiFQS_StripNewLine(string[])
{
	new len = strlen(string);
	if(string[0] == 0)
		return ;

	if((string[len - 1] == '\n') || (string[len - 1] == '\r'))
	{
		string[len - 1] = 0;
		if(string[0]==0)
			return ;

		if((string[len - 2] == '\n') || (string[len - 2] == '\r')) string[len - 2] = 0;
	}
}

stock NiFQS_fcopyTextFile(const oldname[], const newname[])
{
	new File:FirtsArchive, File:SecondArchiver;
	if(!fexist(oldname))
		return false;

	FirtsArchive = fopen(oldname, io_read);
	if(!FirtsArchive)
		return false;

	SecondArchiver = fopen(newname, io_write);
	if(!SecondArchiver)
	{
		fclose(FirtsArchive);
		return false;
	}

	new StrArchiver[NiFQS_MAX_STRING];
	while(fread(FirtsArchive, StrArchiver))
	{
		NiFQS_StripNewLine(StrArchiver);
		format(StrArchiver, sizeof(StrArchiver), "%s\r\n", StrArchiver);
		fwrite(SecondArchiver, StrArchiver);
	}

	fclose(FirtsArchive);
	fclose(SecondArchiver);
	return true;
}


// |--------------- [STOCK LOG] ---------------| //
stock nifqs_LogExists(const filename[])
{
    return fexist(filename);
}

stock nifqs_CreateLog(const File[])
{
    if(fexist(File))
    {
        printf("Use nifqs_WriteLog(\"File\", \"Message\"); To write to an existing log.");
        return 0;
    }
    new File:Archive = fopen(File, io_write);
    fclose(Archive);
    return 1;
}

stock nifqs_WriteLog(const File[], const string[])
{
    if(!fexist(File))
    {
        printf("Use nifqs_CreateLog(\"File\"); o create the log and write to it.");
        return 0;
    }
    new Cellule[300];
    new YearLog, MonthLog, DayLog, HourLog, MinuteLog, SecondLog;
    getdate(YearLog, MonthLog, DayLog);
    gettime(HourLog, MinuteLog, SecondLog);
    format(Cellule, sizeof(Cellule), "(%02d/%02d/%d) - [%02d:%02d:%02d] %s\r\n", DayLog, MonthLog, YearLog, HourLog, MinuteLog, SecondLog, string);
    new File:Archive = fopen(File, io_append);
    fwrite(Archive, Cellule);
    fclose(Archive);
    return 1;
}

/*
* Note: I'm from Brazil, but I decided to write everything in English because it looks much prettier! 
* I'm a Pawno Scripter for the year, I have a lot of experience about this programming language.
* To get in touch with me, just go to my YouTube channel: Nivlekzin.
* I charge for my work, so enjoy while I'm Scripting for free.
*/
