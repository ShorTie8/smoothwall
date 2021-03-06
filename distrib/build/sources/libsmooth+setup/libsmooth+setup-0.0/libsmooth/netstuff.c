/* SmoothWall libsmooth.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: net.c
 * Contains network library functions. */

#include "libsmooth.h"

extern FILE *flog;
extern char *logname;

extern char **ctr;

newtComponent networkform;
newtComponent addressentry;
newtComponent netmaskentry;
newtComponent statictyperadio;
newtComponent dhcptyperadio;
newtComponent pppoetyperadio;
newtComponent dhcphostnameentry;

/* This is a groovie dialog for showing network info.  Takes a keyvalue list,
 * a colour and a dhcp flag.  Shows the current settings, and rewrites them
 * if necessary.  DHCP flag sets wether to show the dhcp checkbox. */
int changeaddress(struct keyvalue *kv, char *colour, int typeflag,
	char *defaultdhcphostname)
{
	const char *addressresult = NULL;
	const char *netmaskresult = NULL;
	const char *dhcphostnameresult = NULL;
	struct newtExitStruct es;
	newtComponent header;
	newtComponent addresslabel;
	newtComponent netmasklabel;
	newtComponent dhcphostnamelabel;
	newtComponent ok, cancel;	
	char message[1000];
	char temp[STRING_SIZE];
	char addressfield[STRING_SIZE];
	char netmaskfield[STRING_SIZE];
	char typefield[STRING_SIZE];
	char dhcphostnamefield[STRING_SIZE];
	int error;
	int result = 0;
	char type[STRING_SIZE];
	int startstatictype = 0;
	int startdhcptype = 0;
	int startpppoetype = 0;
		
	/* Build some key strings. */
	sprintf(addressfield, "%s_ADDRESS", colour);
	sprintf(netmaskfield, "%s_NETMASK", colour);
	sprintf(typefield, "%s_TYPE", colour);
	sprintf(dhcphostnamefield, "%s_DHCP_HOSTNAME", colour);
		
	sprintf(message, ctr[TR_INTERFACE], colour);
	newtCenteredWindow(44, (typeflag ? 18 : 12), message);
	
	networkform = newtForm(NULL, NULL, 0);

	sprintf(message, ctr[TR_ENTER_THE_IP_ADDRESS_INFORMATION], colour);
	header = newtTextboxReflowed(1, 1, message, 42, 0, 0, 0);
	newtFormAddComponent(networkform, header);

	/* See if we need a dhcp checkbox.  If we do, then we shift the contents
	 * of the window down two rows to make room. */
	if (typeflag)
	{
		strcpy(temp, "STATIC"); findkey(kv, typefield, temp);
		if (strcmp(temp, "STATIC") == 0) startstatictype = 1;
		if (strcmp(temp, "DHCP") == 0) startdhcptype = 1;
		if (strcmp(temp, "PPPOE") == 0) startpppoetype = 1;
		statictyperadio = newtRadiobutton(2, 4, ctr[TR_STATIC], startstatictype, NULL);
		dhcptyperadio = newtRadiobutton(2, 5, "DHCP", startdhcptype, statictyperadio);		
		pppoetyperadio = newtRadiobutton(2, 6, "PPPOE", startpppoetype, dhcptyperadio);				
		newtFormAddComponents(networkform, statictyperadio, dhcptyperadio, 
			pppoetyperadio, NULL);
		newtComponentAddCallback(statictyperadio, networkdialogcallbacktype, NULL);
		newtComponentAddCallback(dhcptyperadio, networkdialogcallbacktype, NULL);
		newtComponentAddCallback(pppoetyperadio, networkdialogcallbacktype, NULL);				
		dhcphostnamelabel = newtTextbox(2, 8, 18, 1, 0);
		newtTextboxSetText(dhcphostnamelabel, ctr[TR_DHCP_HOSTNAME]);
		strcpy(temp, defaultdhcphostname);
		findkey(kv, dhcphostnamefield, temp);
		dhcphostnameentry = newtEntry(20, 8, temp, 20, &dhcphostnameresult, 0);
		newtFormAddComponent(networkform, dhcphostnamelabel);		
		newtFormAddComponent(networkform, dhcphostnameentry);	
		if (startdhcptype == 0)
			newtEntrySetFlags(dhcphostnameentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);		
	}
	/* Address */
	addresslabel = newtTextbox(2, (typeflag ? 10 : 4) + 0, 18, 1, 0);
	newtTextboxSetText(addresslabel, ctr[TR_IP_ADDRESS_PROMPT]);
	strcpy(temp, "");
	if (strcmp(colour, "GREEN") == 0)
		strncpy(temp, "192.168.0.1", STRING_SIZE - 1);
	findkey(kv, addressfield, temp);
	addressentry = newtEntry(20, (typeflag ? 10 : 4) + 0, temp, 20, &addressresult, 0);
	if (typeflag == 1 && startstatictype == 0)
		newtEntrySetFlags(addressentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);			
	newtFormAddComponent(networkform, addresslabel);
	newtFormAddComponent(networkform, addressentry);
	
	/* Netmask */
	netmasklabel = newtTextbox(2, (typeflag ? 10 : 4) + 1, 18, 1, 0);
	newtTextboxSetText(netmasklabel, ctr[TR_NETMASK_PROMPT]);
	strcpy(temp, "255.255.255.0"); findkey(kv, netmaskfield, temp);
	netmaskentry = newtEntry(20, (typeflag ? 10 : 4) + 1, temp, 20, &netmaskresult, 0);
	if (typeflag == 1 && startstatictype == 0)
		newtEntrySetFlags(netmaskentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);

	newtFormAddComponent(networkform, netmasklabel);
	newtFormAddComponent(networkform, netmaskentry);

	/* Buttons. */
	ok = newtButton(8, (typeflag ? 13 : 7), ctr[TR_OK]);
	cancel = newtButton(26, (typeflag ? 13 : 7), ctr[TR_CANCEL]);

	newtFormAddComponents(networkform, ok, cancel, NULL);

	newtRefresh();
	newtDrawForm(networkform);

	do
	{
		error = 0;
		newtFormRun(networkform, &es);
	
		if (es.u.co == ok)
		{
			/* OK was pressed; verify the contents of each entry. */
			strcpy(message, ctr[TR_INVALID_FIELDS]);
			
			strcpy(type, "STATIC");
			if (typeflag)
				gettype(type);
			if (strcmp(type, "STATIC") == 0)
			{		
				if (inet_addr(addressresult) == INADDR_NONE)
				{
					strcat(message, ctr[TR_IP_ADDRESS_CR]);
					error = 1;
				}
				if (inet_addr(netmaskresult) == INADDR_NONE)
				{
					strcat(message, ctr[TR_NETWORK_MASK_CR]);
					error = 1;
				}
			}
			if (strcmp(type, "DHCP") == 0)
			{
				if (!strlen(dhcphostnameresult))
				{
					strcat(message, ctr[TR_DHCP_HOSTNAME_CR]);
					error = 1;
				}
			}
			if (error)
				errorbox(message);
			else
			{
				/* No errors!  Set new values, depending on dhcp flag etc. */
				if (typeflag)
				{
					replacekeyvalue(kv, dhcphostnamefield, (char *) dhcphostnameresult);
					if (strcmp(type, "STATIC") != 0)
					{
						replacekeyvalue(kv, addressfield, "0.0.0.0");
						replacekeyvalue(kv, netmaskfield, "0.0.0.0");
					}
					else
					{
						replacekeyvalue(kv, addressfield, (char *) addressresult);		
						replacekeyvalue(kv, netmaskfield, (char *) netmaskresult);					
					}
					replacekeyvalue(kv, typefield, type);					
				}
				else
				{
					replacekeyvalue(kv, addressfield, (char *) addressresult);		
					replacekeyvalue(kv, netmaskfield, (char *) netmaskresult);
				}
				
				setnetaddress(kv, colour);
				result = 1;
			}
		}			
	}
	while (error);

	newtFormDestroy(networkform);
	newtPopWindow();
		
	return result;
}

/* for pppoe: return string thats type STATIC, DHCP or PPPOE */
int gettype(char *type)
{
	newtComponent selected = newtRadioGetCurrent(statictyperadio);
	
	if (selected == statictyperadio)
		strcpy(type, "STATIC");
	else if (selected == dhcptyperadio)
		strcpy(type, "DHCP");
	else if (selected == pppoetyperadio)
		strcpy(type, "PPPOE");
	else
		strcpy(type, "ERROR");
	
	return 0;
}

/* 0.9.9: calculates broadcast too. */
int setnetaddress(struct keyvalue *kv, char *colour)
{
	char addressfield[STRING_SIZE];
	char netaddressfield[STRING_SIZE];		
	char netmaskfield[STRING_SIZE];
	char broadcastfield[STRING_SIZE];
	char address[STRING_SIZE];
	char netmask[STRING_SIZE];
	unsigned long int intaddress;
	unsigned long int intnetaddress;
	unsigned long int intnetmask;
	unsigned long int intbroadcast;
	struct in_addr temp;
	char *netaddress;
	char *broadcast;
		
	/* Build some key strings. */
	sprintf(addressfield, "%s_ADDRESS", colour);
	sprintf(netaddressfield, "%s_NETADDRESS", colour);
	sprintf(netmaskfield, "%s_NETMASK", colour);
	sprintf(broadcastfield, "%s_BROADCAST", colour);

	strcpy(address, ""); findkey(kv, addressfield, address);	
	strcpy(netmask, ""); findkey(kv, netmaskfield, netmask);		

	/* Calculate netaddress. Messy.. */
	intaddress = inet_addr(address);
	intnetmask = inet_addr(netmask);
	
	intnetaddress = intaddress & intnetmask;
	temp.s_addr = intnetaddress;	
	netaddress = inet_ntoa(temp);
	
	replacekeyvalue(kv, netaddressfield, netaddress);
	
	intbroadcast = intnetaddress | ~intnetmask;
	temp.s_addr = intbroadcast;
	broadcast = inet_ntoa(temp);	
	
	replacekeyvalue(kv, broadcastfield, broadcast);
	
	return 1;
}	

/* Called when dhcp flag is toggled.  Toggle disabled state of other 3
 * controls. */
void networkdialogcallbacktype(newtComponent cm, void *data)
{
	char type[STRING_SIZE];
	
	gettype(type);

	if (strcmp(type, "STATIC") != 0)
	{
		newtEntrySetFlags(addressentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);
		newtEntrySetFlags(netmaskentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);
	}
	else
	{
		newtEntrySetFlags(addressentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_RESET);
		newtEntrySetFlags(netmaskentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_RESET);
	}
	if (strcmp(type, "DHCP") == 0)
		newtEntrySetFlags(dhcphostnameentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_RESET);
	else
		newtEntrySetFlags(dhcphostnameentry, NEWT_FLAG_DISABLED, NEWT_FLAGS_SET);		
		
	newtRefresh();
	newtDrawForm(networkform);	
}

int interfacecheck(struct keyvalue *kv, char *colour)
{
	char temp[STRING_SIZE];
	char colourfields[NETCHANGE_TOTAL][STRING_SIZE];
	int c;

	sprintf(colourfields[ADDRESS], "%s_ADDRESS", colour);
	sprintf(colourfields[NETADDRESS], "%s_NETADDRESS", colour);
	sprintf(colourfields[NETMASK], "%s_NETMASK", colour);

	for (c = 0; c < 3; c++)
	{
		strcpy(temp, ""); findkey(kv, colourfields[c], temp);
		if (!(strlen(temp))) return 0;
	}
	return 1;
}

/* Network probing! */
	
#define MAX_NIC_DEVICES 200
#define NIC_DEVICES_FILENAME "/etc/nicdevices"

struct nic nics[MAX_NIC_DEVICES];

int initnicdevices(void)
{
	FILE *hfile = NULL;
	int c = 0;
	char s[STRING_SIZE];
	char *result = NULL;
	int count = 0;

	memset(s, 0, STRING_SIZE);
	memset(&nics, 0, sizeof(struct nic) * MAX_NIC_DEVICES);

	if (!(hfile = fopen(NIC_DEVICES_FILENAME, "r")))
		return -1;
	
	while (fgets(s, STRING_SIZE, hfile) != NULL && c < MAX_NIC_DEVICES - 1)
	{
		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0'; 
		result = strtok(s, "|");
		count = 0;
		
		while (result)
		{
			if (count == 0)
				strncpy(nics[c].description, result, STRING_SIZE - 1);
			else if (count == 1)
				strncpy(nics[c].modulename, result, STRING_SIZE - 1);
			count++;
			result = strtok(NULL, "|");
		}
		c++;
	}

	fclose(hfile);

	return c;
}    

/* Funky routine for loading all drivers (cept those are already loaded.). */
int probecards(char *driver, char *driveroptions, int *pc)
{
	int c;
	char message[1000];
	char commandstring[STRING_SIZE];
	int oldniccount = countcards();
	
	c = *pc;

	while (strlen(nics[c].modulename))
	{
		if (!checkformodule(nics[c].modulename))
		{
			sprintf(commandstring, "/sbin/modprobe %s", nics[c].modulename);
			sprintf(message, ctr[TR_LOOKING_FOR_NIC], nics[c].description);
			if (runcommandwithstatus(commandstring, message) == 0)
			{
				if (countcards() > oldniccount)
				{
					if (newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_SKIP], 
						ctr[TR_SKIP_LONG], nics[c].description) != 1)
					{
						sprintf(commandstring, "/sbin/rmmod %s", nics[c].modulename);
	  					mysystem(commandstring);
  					}
  					else
					{
						strcpy(driver, nics[c].modulename);
						strcpy(driveroptions, "");
						
						/* Next probe run puts us at the next NIC type. */ 
						*pc = c + 1;
						
						return 1;
					}
				}
				else
				{
					/* A "false positive". Remove it. */
					snprintf(commandstring, STRING_SIZE - 1, "/sbin/rmmod %s", nics[c].modulename);
  					mysystem(commandstring);
				}
			}
		}
		c++;
	}
	strcpy(driver, "");
	strcpy(driveroptions, "");
	
	*pc = c;
	
	return 0;
}

/* A listbox for selected the card... with a * MANUAL * entry at top for
 * manual module names. */
int choosecards(char *driver, char *driveroptions)
{
	int c;
	char **sections;
	int drivercount;
	int rc;
	int choice;
	char commandstring[STRING_SIZE];
	char message[STRING_SIZE];
	int done = 0;
	int oldniccount = countcards();
	
	/* Count 'em */
	c = 0; drivercount = 0;
	while (strlen(nics[c].modulename))
	{
		drivercount++;
		c++;
	}
	drivercount++;
	sections = malloc((drivercount + 1) * sizeof(char *));
	
	/* Copy 'em. */
	c = 0;
	sections[c] = ctr[TR_MANUAL];
	c++;
	while (strlen(nics[c - 1].modulename))
	{
		sections[c] = nics[c - 1].description;
		c++;
	}
	sections[c] = NULL;
	
	strcpy(driver, "");
	strcpy(driveroptions, "");
	
	done = 0; choice = 1;
	while (!done)
	{
		rc = newtWinMenu(ctr[TR_SELECT_NETWORK_DRIVER],
			ctr[TR_SELECT_NETWORK_DRIVER_LONG], 50, 5, 5, 6,
			sections, &choice, ctr[TR_OK], ctr[TR_CANCEL], NULL);
		if (rc == 0 || rc == 1)
		{
			if (choice > 0)
			{
				/* Find module number, load module. */
				c = choice - 1;	
				if (!checkformodule(nics[c].modulename))
				{
					sprintf(commandstring, "/sbin/modprobe %s", nics[c].modulename);
					sprintf(message, ctr[TR_LOOKING_FOR_NIC], nics[c].description);
					if (runcommandwithstatus(commandstring, message) == 0)
					{
						if (countcards() > oldniccount)
						{
							strcpy(driver, nics[c].modulename);
							strcpy(driveroptions, "");
							done = 1;
						}
						else
						{
							errorbox(ctr[TR_UNABLE_TO_LOAD_DRIVER_MODULE]);
							/* A "false positive". Remove it. */
							snprintf(commandstring, STRING_SIZE - 1, "/sbin/rmmod %s", nics[c].modulename);
  							mysystem(commandstring);
  						}
					}
					else
						errorbox(ctr[TR_UNABLE_TO_LOAD_DRIVER_MODULE]);
				}
				else
					errorbox(ctr[TR_THIS_DRIVER_MODULE_IS_ALREADY_LOADED]);
			}
			else
			{
				manualdriver(driver, driveroptions);
				if (strlen(driver))
					done = 1;
			}
		}
		else
			done = 1;	
	}

	return 1;
}

/* Manual entry for gurus. */
int manualdriver(char *driver, char *driveroptions)
{
	const char *values[] = { NULL, NULL };	/* pointers for the values. */
	struct newtWinEntry entries[] =
		{ { "", &values[0], 0 }, { NULL, NULL, 0 } };
	int rc;
	char commandstring[STRING_SIZE];
	char *driverend;
	int oldniccount = countcards();

	strcpy(driver, "");
	strcpy(driveroptions, "");
	
	rc = newtWinEntries(ctr[TR_SELECT_NETWORK_DRIVER], 
		ctr[TR_MODULE_PARAMETERS], 50, 5, 5, 40, entries, 
		ctr[TR_OK], ctr[TR_CANCEL], NULL);	
	if (rc == 0 || rc == 1)
	{
		if (strlen(values[0]))
		{
			sprintf(commandstring, "/sbin/modprobe %s", values[0]);
			if (runcommandwithstatus(commandstring, ctr[TR_LOADING_MODULE]) == 0)
			{
				if (countcards() > oldniccount)
				{
					if ((driverend = strchr(values[0], ' ')))
					{
						*driverend = '\0';
						strcpy(driver, values[0]);
						strcpy(driveroptions, driverend + 1);
					}				
					else
					{
						strcpy(driver, values[0]);
						strcpy(driveroptions, "");
					}
				}
				else
				{
					errorbox(ctr[TR_UNABLE_TO_LOAD_DRIVER_MODULE]);
					/* Should probably remove the module here. */
				}
			}
			else
				errorbox(ctr[TR_UNABLE_TO_LOAD_DRIVER_MODULE]);
		}
		else
			errorbox(ctr[TR_MODULE_NAME_CANNOT_BE_BLANK]);
	}
	free((char *) values[0]);

	return 1;
}

/* Returns the total number of nics current available as ethX devices. */
int countcards(void)
{
 	FILE *file;
	char buffer[STRING_SIZE];
	char *start;
	int niccount = 0;
	
	if (!(file = fopen("/proc/net/dev", "r")))
	{
		fprintf(flog, "Unable to open /proc/net/dev in countcards()\n");
		return 0;
	}
	
	while (fgets(buffer, STRING_SIZE, file))
	{
		start = buffer;
		while (*start == ' ') start++;
		if (strncmp(start, "eth", strlen("eth")) == 0)
			niccount++;
	}
	
	fclose(file);
	
	return niccount;
}

/* Finds the listed module name and copies the card description back. */
int findnicdescription(char *modulename, char *description)
{
	int c = 0;
	
	while (strlen(nics[c].description))
	{
		if (strcmp(nics[c].modulename, modulename) == 0)
		{
			strcpy(description, nics[c].description);
			return 1;
		}
		c++;
	}
	
	strcpy(description, "UNKNOWN");

	return 0;
}

/* Returns MAC address string given the device. */
int getnicmac(char *dest, int size, char *dev)
{
	int fd = 0;
	int found = 0;
	struct ifreq req;

	strcpy(dest, "");

	/* Create socket to do ioctl. */
	if ((fd = socket(PF_INET, SOCK_DGRAM, 0)))
	{
		/* Set ifrn_name to device name eg. "eth0" */
		snprintf(req.ifr_ifrn.ifrn_name, IFNAMSIZ, dev);

		/* Do ioctl to get hardware address (MAC) */
		if (!(ioctl(fd, SIOCGIFHWADDR, &req)))
		{
			/* Format MAC into colon seperated string format. */
			snprintf(dest, size, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
			req.ifr_ifru.ifru_hwaddr.sa_data[0] & 0xff, 
			req.ifr_ifru.ifru_hwaddr.sa_data[1] & 0xff, 
			req.ifr_ifru.ifru_hwaddr.sa_data[2] & 0xff, 
			req.ifr_ifru.ifru_hwaddr.sa_data[3] & 0xff, 
			req.ifr_ifru.ifru_hwaddr.sa_data[4] & 0xff, 
			req.ifr_ifru.ifru_hwaddr.sa_data[5] & 0xff);
			found = 1;
		}
	}

	if (fd) close(fd);

	return (found);
}
