/*****************************************************
 *                                                   *
 *             NiFQS - File Query System             *
 *                                                   *
 *         @Author        : Kelvin/Nivlekzin         *
 *         @Date          : 07th Dec 2022            *
 *         @Time          : 00h 00m 00s PM           *
 *         @Update        : 07th Dec 2022            *
 *         @Version       : 1.0.0                    *
 *                                                   *
 *              Created By Nivlekzin :D              *
 *                                                   *
 *****************************************************/

/*
*    Meaning of the acronym NiFQS
*    Ni = Nivlekzin
*    FQS = File Query System
*    Ni + FQS = NiFQS
*/

/*
native nifqs_Start();
native nifqs_Exit();
native nifqs_FileExists(const filename[]);
native nifqs_RemoveFile(const filename[]);
native nifqs_CreateFile(const filename[]);
native nifqs_SetString(const filename[], const key[], const value[]);
native nifqs_GetString(const filename[], const key[]);
native nifqs_SetInt(const filename[], const key[], value);
native nifqs_GetInt(const filename[], const key[]);
native nifqs_SetBool(const filename[], const key[], Bool:value);
native nifqs_GetBool(const filename[], const key[]);
native nifqs_SetFloat(const filename[], const key[], Float:value);
native nifqs_GetFloat(const filename[], const key[]);
native nifqs_UnSet(const filename[], const key[]);
native nifqs_IsSet(const filename[], const key[]);
native nifqs_LogExists(const filename[]);
native nifqs_RemoveLog(const filename[]);
native nifqs_CreateLog(const filename[]);
native nifqs_WriteLog(const filename[], const string[]);
*/

/*
*    ATTENTION: Do not modify anything in Include/Script!
*    Otherwise, you could suffer serious problems. Do not remove credits, do not modify anything.
*/


// |---------------- [DEFINES] ----------------| //
#if defined _nifqs_included
    #endinput
#endif

#if !defined _samp_included
    #error Please include <a_npc> or <a_samp> first.
#endif

#define _nifqs_included
#pragma library NiFQS

#if defined MAX_STRING
    #define NiFQS_MAX_STRING    MAX_STRING
#else
    #define NiFQS_MAX_STRING    255
#endif


// |----------------- [NiFQS] -----------------| //
// Use the stock 'nifqs_Start();' in public 'OnGameModInit();'.
nifqs_Start()
{
    print("\n\nStarting the NiFQS Database...");
    print("NiFQS database started successfully!\n");
    print("\n*****************************************************");
    print("*                                                   *");
    print("*             NiFQS - File Query System             *");
    print("*                                                   *");
    print("*         @Author        : Kelvin/Nivlekzin         *");
    print("*         @Date          : 07th Dec 2022            *");
    print("*         @Time          : 00h 00m 00s PM           *");
    print("*         @Update        : 07th Dec 2022            *");
    print("*         @Version       : 1.0.0                    *");
    print("*                                                   *");
    print("*              Created By Nivlekzin :D              *");
    print("*                                                   *");
    print("*****************************************************\n");
    return 0;
}

// Use the stock 'nifqs_Exit();' in public 'OnGameModExit();'.
nifqs_Exit()
{
    print("*****************************************************");
    print("*                                                   *");
    print("*             NiFQS - File Query System             *");
    print("*                                                   *");
    print("* Closing the NiFQS database...                     *");
    print("* The NiFQS database was closed successfully!       *");
    print("*                                                   *");
    print("*****************************************************");
    return 0;
}


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
        return 0;
    
    new File:stctf;
    stctf = fopen(filename, io_write);
    if(stctf)
    {
        fclose(stctf);
        return 1;
    }
    return 0;
}

/*
stock information <nifqs_SetString>
If there is no key, it cannot be set.
We cannot set the value, if all together they are greater than the maximum string.
the inventory can only receive values of type characters.
If an old *.part file already exists, we will remove it.
*/
stock nifqs_SetString(const filename[], const key[], const value[])
{
    new key_length = strlen(key);
    new value_length = strlen(value);
    if(key_length == 0 || key_length + value_length + 2 > NiFQS_MAX_STRING)
        return 0;

    new File:otftwi, File:Wttftwo;
    new sstbw[NiFQS_MAX_STRING];
    new bool:citfws = false;

    format(sstbw, sizeof(sstbw), "%s.part", filename);
    fremove(sstbw);

    otftwi = fopen(filename, io_read);
    if(!otftwi) return 0;

    Wttftwo = fopen(sstbw, io_write);
    if(!Wttftwo)
    {
        fclose(otftwi);
        return 0;
    }

    while(fread(otftwi, sstbw))
    {
        if(!citfws && sstbw[key_length] == '=' && !strcmp(sstbw, key, true, key_length))
        {
            format(sstbw, sizeof(sstbw), "%s=%s", key, value);
            citfws = true;
        }
        else
        {
            NiFQS_StripNewLine(sstbw);
        }
        fwrite(Wttftwo, sstbw);
        fwrite(Wttftwo, "\r\n");
    }

    if(!citfws)
    {
        format(sstbw, sizeof(sstbw), "%s=%s", key, value);
        fwrite(Wttftwo, sstbw);
        fwrite(Wttftwo, "\r\n");
    }

    fclose(otftwi);
    fclose(Wttftwo);

    format(sstbw, sizeof(sstbw), "%s.part", filename);
    if(NiFQS_fcopyTextFile(sstbw, filename))
    {
        return fremove(sstbw);
    }
    return 0;
}

stock nifqs_GetString(const filename[], const key[])
{
    new sstbw[NiFQS_MAX_STRING];

    new key_length = strlen(key);
    if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
        return sstbw;

    new File:otftwi;
    otftwi = fopen(filename, io_read);
    if(!otftwi)
        return sstbw;
    
    while(fread(otftwi, sstbw))
    {
        if(sstbw[key_length] == '=' && !strcmp(sstbw, key, true, key_length))
        {
            NiFQS_StripNewLine(sstbw);
            strmid(sstbw, sstbw, key_length + 1, strlen(sstbw), NiFQS_MAX_STRING);
            fclose(otftwi);
            return sstbw;
        }
    }

    fclose(otftwi);
    return sstbw;
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

stock nifqs_SetBool(const filename[], const key[], bool:value)
{
    if(value == true)
    {
        return nifqs_SetString(filename, key, "1");
    }
    return nifqs_SetString(filename, key, "0");
}

stock nifqs_GetBool(const filename[], const key[])
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

stock nifqs_UnSet(const filename[], const key[])
{
    new key_length = strlen(key);
    if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
        return 0;
    
    new File:otftwi, File:Wttftwo;
    new sstbw[NiFQS_MAX_STRING];

    format(sstbw, NiFQS_MAX_STRING, "%s.part", filename);
    fremove(sstbw);

    otftwi = fopen(filename, io_read);
    if(!otftwi)
        return 0;

    Wttftwo = fopen(sstbw, io_write);
    if(!Wttftwo)
    {
        fclose(otftwi);
        return 0;
    }

    while(fread(otftwi, sstbw))
    {
        if(sstbw[key_length] == '=' && !strcmp(sstbw, key, true, key_length))
        {
            // We already have what needs to be removed!
        }
        else
        {
            NiFQS_StripNewLine(sstbw);
            fwrite(Wttftwo, sstbw);
            fwrite(Wttftwo, "\r\n");
        }
    }

    fclose(otftwi);
    fclose(Wttftwo);

    format(sstbw, NiFQS_MAX_STRING, "%s.part", filename);
    if(NiFQS_fcopyTextFile(sstbw, filename))
    {
        return fremove(sstbw);
    }
    return 0;
}

stock nifqs_IsSet(const filename[], const key[])
{
    new key_length = strlen(key);
    if(key_length == 0 || key_length + 2 > NiFQS_MAX_STRING)
        return 0;

    new File:otftwi;
    otftwi = fopen(filename, io_read);
    if(!otftwi)
        return 0;

    new sstbw[NiFQS_MAX_STRING];
    while(fread(otftwi, sstbw))
    {
        if(sstbw[key_length] == '=' && !strcmp(sstbw, key, true, key_length))
        {
            fclose(otftwi);
            return 1;
        }
    }

    fclose(otftwi);
    return 0;
}

stock nifqs_LogExists(const filename[])
{
    return fexist(filename);
}

stock nifqs_RemoveLog(const filename[])
{
    return fremove(filename);
}

stock nifqs_CreateLog(const filename[])
{
    if(fexist(filename))
    {
        printf("Use nifqs_WriteLog(\"File\", \"Message\"); to write to an existing log.");
        return 0;
    }
    new File:Archiver = fopen(filename, io_write);
    fclose(Archiver);
    return 0;
}

stock nifqs_WriteLog(const filename[], const string[])
{
    if(!fexist(filename))
    {
        printf("Use nifqs_CreateLog(\"File\"); to create the Log and write to it.");
        return 0;
    }
    new cells[ 300 ];
    new yearlog, monthlog, daylog, hourlog, minutelog, secondlog;
    getdate(yearlog, monthlog, daylog);
    gettime(hourlog, minutelog, secondlog);
    format(cells, sizeof(cells), "(%02d/%02d/%d) - [%02d:%02d:%02d] | %s\r\n", daylog, monthlog, yearlog, hourlog, minutelog, secondlog, string);
    new File:Archiver = fopen(filename, io_append);
    fwrite(Archiver, cells);
    fclose(Archiver);
    return 0;
}



stock NiFQS_StripNewLine(String[])
{
    new len = strlen(String);
    if(String[0] == 0)
        return ;
    if((String[len - 1] == '\n') || (String[len - 1] == '\r'))
    {
        String[len - 1] = 0;
        if(String[0] == 0)
            return ;
        if((String[len - 2] == '\n') || (String[len - 2] == '\r')) String[len - 2] = 0;
    }
}

stock NiFQS_fcopyTextFile(const oldname[], const newname[])
{
    new File:otftwi, File:Wttftwo;
    if(!fexist(oldname))
        return 0;
    
    otftwi = fopen(oldname, io_read);
    if(!otftwi)
        return 0;

    Wttftwo = fopen(newname, io_write);
    if(!Wttftwo)
    {
        fclose(otftwi);
        return 0;
    }

    new sstbw[NiFQS_MAX_STRING];
    while(fread(otftwi, sstbw))
    {
        NiFQS_StripNewLine(sstbw);
        format(sstbw, sizeof(sstbw), "%s\r\n", sstbw);
        fwrite(Wttftwo, sstbw);
    }

    fclose(otftwi);
    fclose(Wttftwo);
    return 1;
}
