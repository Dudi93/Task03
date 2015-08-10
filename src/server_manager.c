#include <stdio.h>
#include "server_manager.h"

#include "user.h"

int parStart;

/*void parser(char *buff)
{
	msg=buff;
	parStart=0;
	int msg_len = msg[parStart];
	if(msg_len<=1)
	{
		printf("Msg too short, d/t understand");
		return;
	}
	parStart+=8;
	command=msg[parStart];
	while(parStart<msg_len+8){
		parStart++;
		printf("%c",msg[parStart]);
	}
	switch(command)
	{
	//LOGIN
	case 1:
		//try/throw:
		//-jesli wczesniej nie zalogowany - sendErr();
		//-jesli nazwa zajeta - sendErr();
		//-jesli nie ma miejsca - won + sendErr();
		//-jesli ok - add_user/sendOk();
		break;
	//sndMsg
	case 2:
		//try/throw:
		//-jesli empty po case - sendErr();
		//-jesli ok- poki co write back to usr
		break;
	default:
		//zjebales numerki/oznaczenia/fszysko
		break;
	}

}*/
