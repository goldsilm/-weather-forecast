#include<stdio.h>
#include<stdlib.h>
#include"cJSON.h"
#include<string.h>
#include <curl/curl.h> 
#define NONE         "\033[m"  
#define RED          "\033[0;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define LIGHT_YELLOW "\033[0;33m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"
struct MemoryStruct 
{
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)    // Injecting information into cache
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL)
	{
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

int main(int argc, char *argv[])
{
	int i;
	char key[40] = {"0"};
	char location[20] = { "Beijing" };
	for(i=2;i<=argc;i++)
    {
		if(strcmp(argv[i-1], "-help")==0)
		{
			printf(" -l [location] if you do not choose,it will be Beijing\n");
			printf(" -k [key]  It is necessary,you can get a free key from \"https://developer.worldweatheronline.com\"\n");
		        exit(0);
		}
		else if(strcmp(argv[i-1], "-l")==0)
		{
			strcpy(location, argv[i]);
			i=i+1;		
		}
		else if(strcmp(argv[i-1], "-k")==0)
		{
			strcpy(key, argv[i]);
			i=i+1;	
		}
		else
		{
			printf("You can imput -help to get help\n");
			exit(0);
		}
	}
	if (strlen(key) <= 8)             //check the key
	{
		printf("Please imput right key \n");
		exit(0);
	}
	char url[150] = "http://api.worldweatheronline.com/premium/v1/weather.ashx?key=";
	strcat(url,key);
	strcat(url, "&q=");
	strcat(url, location);
	strcat(url, "&format=json&num_of_days=3");
	int judedata(char data[]);                                       //judgedata
	void weatherprint(char data[]);                                //weather print
	CURL *curl;                                                     //star get
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */
	/* init the curl session */
	curl = curl_easy_init();
	/* specify URL to get */
	curl_easy_setopt(curl, CURLOPT_URL,url);
	/* send all data to this function  */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	/* get it! */
	res = curl_easy_perform(curl);
	/* check for errors */
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		exit(0);
	}
	i=judedata(chunk.memory);
	if (i==1)
	{
		exit(0);
	}
	weatherprint(chunk.memory);
	/* cleanup curl stuff */
	curl_easy_cleanup(curl);
	free(chunk.memory);
	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();
	return 0;
}
int judgeweather(char *x)        //judge the weather and return the figure
{
	if (strcmp(x, "Overcast") == 0)
		return 1;
	else if (strcmp(x, "Sunny") == 0 || strcmp(x, "Clear") == 0)
		return 2;
	else if (strcmp(x, "Light rain") == 0 || strcmp(x, "Patchy rain possible") == 0 || strcmp(x, "Light rain shower") == 0 || strcmp(x, "Light drizzle") == 0 || strcmp(x, "Patchy light rain") == 0 || strcmp(x, "Light Rain Shower") == 0)
		return 3;
	else if (strcmp(x, "medium rain") == 0)
		return 4;
	else if (strcmp(x, "big rain") == 0)
		return 5;
	else if (strcmp(x, "Moderate rain at times") == 0)
		return 6;
	else if (strcmp(x, "Partly cloudy") == 0)
		return 7;
	else if (strcmp(x, "rainstorm") == 0)
		return 8;
	else if (strcmp(x, "Cloudy") == 0)
		return 9;
	else if (strcmp(x, "Moderate or heavy rain shower") == 0)
		return 10;
	else if (strcmp(x, "Thundery outbreaks possible") == 0 || strcmp(x, "Patchy light rain with thunder") == 0 || strcmp(x, "Light Rain With Thunderstorm, Mist") == 0)
		return 11;
	else if (strcmp(x, "Light Rain With Thunderstorm") == 0)
		return 12;
	else
		return 0;
}
void message(cJSON *node, int x, int y, int z)       //print message of weather  ; x is the day  ; y is the list ; z  is the  line
{
	void colorspeed(cJSON *data);
	void colortemp(cJSON *data);
	cJSON *node1;
	cJSON *node2;
	cJSON *node3;
	cJSON *node4;
	cJSON *node5;
	cJSON *node6;
	cJSON *node7;
	int a, b;
	if (y <= 2)
		a = 2 * y + 2;
	else
		a = 0;
	node1 = cJSON_GetArrayItem(node, x);
	node2 = cJSON_GetObjectItem(node1, "hourly");
	node3 = cJSON_GetArrayItem(node2, a);
	node4 = cJSON_GetArrayItem(node2, a + 1);
	node5 = cJSON_GetObjectItem(node3, "weatherDesc");
	node6 = cJSON_GetArrayItem(node5, 0);
	node7 = cJSON_GetObjectItem(node6, "value");
	b = judgeweather(node7->valuestring);
	picture(b, z);
	if (z == 0)
		printf(" %-12.12s│", node7->valuestring);
	else if (z == 1)
	{
		node5 = cJSON_GetObjectItem(node3, "tempC");
		node6 = cJSON_GetObjectItem(node4, "tempC");
		if (strlen(node5->valuestring)<strlen(node6->valuestring))
		{
			colortemp(node5);
			printf("-");
			colortemp(node6);
			printf("°C   │");
		}
		else if (strlen(node5->valuestring) == strlen(node6->valuestring))
		{
			if (strcmp(node5->valuestring, node6->valuestring) > 0)
			{
				colortemp(node6);
				printf("-");
				colortemp(node5);
				printf("°C   │");
			}
			else
			{
				colortemp(node5);
				printf("-");
				colortemp(node6);
				printf("°C   │");
			}

		}
		else
		{
			colortemp(node6);
			printf("-");
			colortemp(node5);
			printf("°C   │");
		}

	}
	else if (z == 2)
	{
		node5 = cJSON_GetObjectItem(node3, "windspeedKmph");
		node6 = cJSON_GetObjectItem(node4, "windspeedKmph");
		node7 = cJSON_GetObjectItem(node4, "winddir16Point");
		if (strlen(node5->valuestring)<strlen(node6->valuestring))
		{
			printf("%3s ", node7->valuestring);
			colorspeed(node5);
			printf("-");
			colorspeed(node6);
			printf("km/h│");
		}
		else if (strlen(node5->valuestring) == strlen(node6->valuestring))
		{
			if (strcmp(node5->valuestring, node6->valuestring) > 0)
			{
				printf("%3s ", node7->valuestring);
				colorspeed(node6);
				printf("-");
				colorspeed(node5);
				printf("km/h│");
			}
			else
			{
				printf("%3s ", node7->valuestring);
				colorspeed(node5);
				printf("-");
				colorspeed(node6);
				printf("km/h│");
			}

		}
		else
		{
			printf("%3s ", node7->valuestring);
			colorspeed(node6);
			printf("-");
			colorspeed(node5);
			printf("km/h│");
		}
	}
	else if (z == 3)
	{
		node5 = cJSON_GetObjectItem(node3, "visibility");
		printf("%3s km       │", node5->valuestring);
	}
	else
	{
		node5 = cJSON_GetObjectItem(node3, "precipMM");
		node6 = cJSON_GetObjectItem(node4, "humidity");
		printf("%4s mm| %2s%% │", node5->valuestring, node6->valuestring);
	}
	if (y == 3 && z != 4)
		printf("\n│");
	if (y == 3 && z == 4)
		printf("\n");
}
void date(cJSON *node, int x)         //date of weather
{
	cJSON *node1;
	cJSON *node2;
	node1 = cJSON_GetArrayItem(node, x);
	node2 = cJSON_GetObjectItem(node1, "date");
	printf("  %s ", node2->valuestring);
}

int judedata(char data[])                  // jude data
{
	cJSON *json;
	if (strlen(data) > 160)
	{
		cJSON *json = cJSON_Parse(data);
		if (json == NULL)
		{
           printf("ERROR:data corruption\n");
			exit(0);
		}
		return 0;
	}
	else if (strlen(data) < 160 && strlen(data) >5)
	{
		cJSON *node;
		cJSON *node1;
		cJSON *node2;
		cJSON *node3;
		json = cJSON_Parse(data);
		if (json == NULL)
		{
			printf("ERROR:data corruption\n");
			exit(0);
		}
		node = cJSON_GetObjectItem(json, "data");
		node1 = cJSON_GetObjectItem(node, "error");
		node2 = cJSON_GetArrayItem(node1, 0);
		node3 = cJSON_GetObjectItem(node2, "msg");
		printf("error:%s\n", node3->valuestring);
		return 1;
	}
	else
	{
		printf("Unconnected Network!\n");
		exit(0);
	}
}
void colortemp(cJSON *data)                   //let temp in color
{
	if (strlen(data->valuestring) == 2)
	{
		if (strcmp(data->valuestring, "-99") > 0)
		{
			if (strcmp(data->valuestring, "23") >= 0 && strcmp(data->valuestring, "29") <= 0)
			{

				printf(LIGHT_YELLOW"%3.3s"NONE, data->valuestring);

			}
			else if (strcmp(data->valuestring, "29") > 0)
			{
				printf(LIGHT_RED"%3.3s"NONE, data->valuestring);
			}
			else
			{
				printf(YELLOW"%3.3s"NONE, data->valuestring);
			}
		}
		else
		{
			printf(BLUE"%3.3s"NONE, data->valuestring);
		}
	}
	else if (strlen(data->valuestring) == 1)
	{
		printf("%3.3s", data->valuestring);
	}
	else
	{

		printf(LIGHT_BLUE"%3.3s"NONE, data->valuestring);
	}
}
void colorspeed(cJSON *data)                   //let windspeed in color
{
	if (strlen(data->valuestring) == 2)
	{
		if (strcmp(data->valuestring, "10") >= 0 && strcmp(data->valuestring, "16") <= 0)
		{
			printf(LIGHT_YELLOW"%2.2s"NONE, data->valuestring);
		}
		else if (strcmp(data->valuestring, "16") > 0 && strcmp(data->valuestring, "23") <= 0)
		{
			printf(YELLOW"%2.2s"NONE, data->valuestring);
		}
		else if (strcmp(data->valuestring, "23") > 0)
		{
			printf(LIGHT_RED"%2.2s"NONE, data->valuestring);
		}
		else
		{
			printf("%2.2s", data->valuestring);
		}
	}
	else
	{
		printf("%2.2s", data->valuestring);
	}
}
void weatherprint(char data[])              //print weather
{
	void message(cJSON *node, int x, int y, int z);
	cJSON *json;
	cJSON *node;
	cJSON *node1;
	cJSON *node2;
	cJSON *node3;
	cJSON *node4;
	cJSON *node5;
	int i;
	int j;
	json = cJSON_Parse(data);
	if (json == NULL)
	{
		printf("Error(0) the data of json is not right");
		exit(0);
	}
	node = cJSON_GetObjectItem(json, "data");                       //the top of weather
	node1 = cJSON_GetObjectItem(node, "request");
	node2 = cJSON_GetArrayItem(node1, 0);
	node3 = cJSON_GetObjectItem(node2, "query");
	printf("City:%s\n", node3->valuestring);
	node1 = cJSON_GetObjectItem(node, "current_condition");
	node2 = cJSON_GetArrayItem(node1, 0);
	node3 = cJSON_GetObjectItem(node2, "weatherDesc");
	node4 = cJSON_GetArrayItem(node3, 0);
	node5 = cJSON_GetObjectItem(node4, "value");
	i = judgeweather(node5->valuestring);
	picture(i, 0);
	printf("  %s\n", node5->valuestring);
	picture(i, 1);
	node1 = cJSON_GetObjectItem(node, "current_condition");
	node2 = cJSON_GetArrayItem(node1, 0);
	node3 = cJSON_GetObjectItem(node2, "temp_C");
	printf("  ");
	colortemp(node3);
	printf("°C \n");
	picture(i, 2);
	node3 = cJSON_GetObjectItem(node2, "winddir16Point");
	printf("  %s", node3->valuestring);
	node3 = cJSON_GetObjectItem(node2, "windspeedKmph");
	printf(" ");
	colorspeed(node3);
	printf(" km/h\n");
	picture(i, 3);
	node3 = cJSON_GetObjectItem(node2, "visibility");
	printf("  %s km\n", node3->valuestring);
	picture(i, 4);
	node3 = cJSON_GetObjectItem(node2, "precipMM");
	printf("  %smm\n", node3->valuestring);
	node1 = cJSON_GetObjectItem(node, "weather");
	printf("                                                   ┌─────────────┐\n");
	printf("┌────────────────────────────┬─────────────────────┤");
	date(node1, 0);
	printf("├─────────────────────┬────────────────────────────┐\n");
	printf("│          Morning           │           Noon      └──────┬──────┘    Evening          │            Night           │\n");
	printf("├────────────────────────────┼────────────────────────────┼────────────────────────────┼────────────────────────────┤\n");
	printf("│");
	for (i = 0; i < 5; i++)
		for (j = 0; j < 4; j++)
			message(node1, 0, j, i);
	printf("└────────────────────────────┴────────────────────────────┴────────────────────────────┴────────────────────────────┘\n\n");
	printf("                                                   ┌─────────────┐\n");
	printf("┌────────────────────────────┬─────────────────────┤");
	date(node1, 1);
	printf("├─────────────────────┬────────────────────────────┐\n");
	printf("│          Morning           │           Noon      └──────┬──────┘    Evening          │            Night           │\n");
	printf("├────────────────────────────┼────────────────────────────┼────────────────────────────┼────────────────────────────┤\n");
	printf("│");
	for (i = 0; i < 5; i++)
		for (j = 0; j < 4; j++)
			message(node1, 1, j, i);
	printf("└────────────────────────────┴────────────────────────────┴────────────────────────────┴────────────────────────────┘\n\n");
	printf("                                                   ┌─────────────┐\n");
	printf("┌────────────────────────────┬─────────────────────┤");
	date(node1, 2);
	printf("├─────────────────────┬────────────────────────────┐\n");
	printf("│          Morning           │           Noon      └──────┬──────┘    Evening          │            Night           │\n");
	printf("├────────────────────────────┼────────────────────────────┼────────────────────────────┼────────────────────────────┤\n");
	printf("│");
	for (i = 0; i < 5; i++)
		for (j = 0; j < 4; j++)
			message(node1, 2, j, i);
	printf("└────────────────────────────┴────────────────────────────┴────────────────────────────┴────────────────────────────┘\n\n");
	printf("\n");
}

	
