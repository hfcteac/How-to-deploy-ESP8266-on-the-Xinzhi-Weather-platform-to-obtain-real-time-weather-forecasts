#define BLINKER_WIFI
#include <ArduinoJson.h>
#include <Blinker.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "UTF-8toGB2312.h"

#define TEXTE_1 "tex-1"    // 窗帘开户状态
#define TEXTE_2 "tex-2"    // 窗帘状态
#define Slider_1 "TempKey" // 窗帘开启时间
#define Slider_2 "TempK"   // 窗帘关闭时间

String xdata;
char *leftover;
long number, number2;

char auth[] = "123456789876";
const char ssid[] = "TCL-578101";
const char pass[] = "12345678";

const char *host = "116.62.81.138";
const uint16_t port = 80;

// 用于API请求的标志和结果（你在WeatherTask和loop中用到了）
volatile int api_request_flag = 0; // 0无请求，1笑话，2早安，3故事
String api_result = "";            // 存储API返回内容

String City = "WXRTZ3E3MWSD"; // 沈阳铁西
String My_Key = "SqckyEc0s9034nHrb";

static const char ntpServerName[] = "time.nist.gov";
const int timeZone = 8;
int years, months, days, hours, minutes, seconds, weekdays, ones, tens;

WiFiUDP Udp;
unsigned int localPort = 8888;

// 添加天气更新控制变量
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 10000; // 10秒更新一次

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
void oledClockDisplay();

boolean isNTPConnected = false;

typedef struct
{
    int temperature;
    int pressure;
    int shidu;
    int tianqitubiao;
    int fengjiaodu;
    int dengji;
} tianqixinxi;

tianqixinxi day1 = {0, 0, 0, 0, 0, 0}; // 初始化
int OnTime = -1;
bool DatFlag = true;
unsigned long getTime = 0;
String inputString = "";

String url = "/v3/weather/now.json?key=" + My_Key + "&location=" + City + "&language=zh-Hans&unit=c";
String urlDat = "key=" + My_Key + "&location=" + City + "&language=zh-Hans&unit=c";

int led1 = 0;
int led2 = 0;
long int cl = 0;
int opentime = 7;
int closetime = 18;
int slide = 0;                  // 这个是滑动条的控制
BlinkerButton Button1("btn-1"); // 窗
BlinkerButton Button2("btn-2"); // 窗帘
BlinkerSlider Slider1(Slider_1);
BlinkerSlider Slider2(Slider_2);
BlinkerText Text1(TEXTE_1); // 窗帘开户状态
BlinkerText Text2(TEXTE_2); // 窗帘状态
BlinkerNumber edw("edw");

// 双核任务句柄
TaskHandle_t WeatherTaskHandle = NULL;

void button1_callback(const String &state)
{
    led1++;
    if (led1 > 1)
        led1 = 0;
    if (led1 == 1)
        cl += 1;
    else if (led1 == 0)
        cl -= 1;
}

void button2_callback(const String &state)
{
    led2++;
    if (led2 > 1)
        led2 = 0;
    if (led2 == 1)
        cl += 10;
    else if (led2 == 0)
        cl -= 10;
}

void slider1_callback(int32_t val)
{
    opentime = val;
}

void slider2_callback(int32_t val2)
{
    closetime = val2;
}

void heartbeat()
{
    edw.print(number2);
}

// 核心0任务 - 专门负责天气请求
void WeatherTask(void *pvParameters)
{
    while (1)
    {
        // 天气请求
        if (millis() - lastWeatherUpdate >= weatherUpdateInterval)
        {
            lastWeatherUpdate = millis();
            GET_Weather_Blocking();
        }

        // 处理API请求
        if (api_request_flag == 1)
        {
            // TODO: 这里请求笑话API
            // api_result = getJokeFromAPI();
            //api_result = "这里是笑话内容"; // 测试用
            api_result = getJokeFromAPI();
            api_request_flag = 0;
        }
        else if (api_request_flag == 2)
        {
            // TODO: 这里请求早安API
            // api_result = getMorningFromAPI();
            //api_result = "这里是早安内容"; // 测试用
            api_result = getMorningFromAPI();
            api_request_flag = 0;
        }
        else if (api_request_flag == 3)
        {
            // TODO: 这里请求故事API
            // api_result = getStoryFromAPI();
            //api_result = "这里是故事内容"; // 测试用
            api_result = getStoryFromAPI();
            api_request_flag = 0;
        }

        // 使用查表法转换UTF-8到GB2312
            if (api_result.length() > 0)
            {
                api_result.replace("\r", "");
                api_result.replace("\n", "");
                
                // 限制长度，防止内存溢出
                if (api_result.length() > 500) {
                    api_result = api_result.substring(0, 500);
                }
                
                int num = api_result.length();
                
                // 使用堆内存分配，避免栈溢出
                unsigned char* s = (unsigned char*)malloc(num * 3);  // 预留更多空间
                
                if (s != NULL) {  // 检查内存分配是否成功
                    int len = Utf8ToGb2312(api_result.c_str(), num, s);
                    
                    // 发送GB2312字节
                    Serial.write((const uint8_t*)"china", 5);
                    Serial.write(s, len);
                    Serial.write((const uint8_t*)"\r\n", 2);
                    
                    free(s);  // 释放内存
                }
                
                api_result = "";
            }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(9600);
    delay(100);

    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    delay(200);

    digitalWrite(2, HIGH);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20)
    {
        delay(500);
        Serial.print(".");
        timeout++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Udp.begin(localPort);
        setSyncProvider(getNtpTime);
        setSyncInterval(300);
    }

    Blinker.begin(auth, ssid, pass);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Slider1.attach(slider1_callback);
    Slider2.attach(slider2_callback);
    Blinker.attachHeartbeat(heartbeat);

    // 创建天气任务,运行在核心0
    BaseType_t result = xTaskCreatePinnedToCore(
        WeatherTask,
        "WeatherTask",
        40000,
        NULL,
        1,
        &WeatherTaskHandle,
        0);

    if (result == pdPASS)
    {
        // Serial.println("[Setup] 天气任务创建成功!");
    }
    else
    {
        // Serial.println("[Setup] 天气任务创建失败!");
    }
}

// 核心1主循环 - 负责数据发送和Blinker通信
void loop()
{
    Blinker.run();
    oledClockDisplay();
    xdata = receive();
    if (xdata != "")
    {
        // 只处理 jokes、morning、story 三个命令
        if (xdata.indexOf("jokes") != -1)
        {
            api_request_flag = 1; // 请求笑话
        }
        else if (xdata.indexOf("morning") != -1)
        {
            api_request_flag = 2; // 请求早安
        }
        else if (xdata.indexOf("story") != -1)
        {
            api_request_flag = 3; // 请求故事
        }
        else
        {
            // 其他内容才执行原有的 number、number2、Text1、Text2 等逻辑
            char *tmp = nullptr;
            number = strtoul(xdata.c_str(), &tmp, 10);
            if (tmp && *tmp)
            {
                number2 = atoi(tmp);
            }
            else
            {
                number2 = 0;
            }
        }
        if (number % 10 == 1)
            Text1.print("窗户已开启");
        else
            Text1.print("窗户已关闭");
        if (number / 10 % 10 == 1)
            Text2.print("窗帘已展开");
        else
            Text2.print("窗帘已收起");
    }

    Serial.printf("%d%02d%02d%01d%02d %d %02d%02d %03d%02d %d %d %d %d%d/%02d/%02d-%02d:%02d \r\n",
                  WiFi.status(), opentime, closetime, Blinker.connected(), cl, hours, day1.tianqitubiao, day1.shidu,
                  day1.fengjiaodu, day1.dengji, day1.temperature, day1.pressure, slide, tens, ones,
                  months, days, hours, minutes);
    digitalWrite(2, Blinker.connected());
    delay(500);
    yield();
}

String receive()
{
    String data = "";
    while (Serial.available())
    {
        char c = (char)Serial.read();
        data += c;
        delay(2);
    }
    return data;
}

// 核心0使用的阻塞式天气请求
void GET_Weather_Blocking()
{
    WiFiClient client;
    client.setTimeout(5000);

    if (!client.connect(host, port))
    {
        return;
    }

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.connected() && !client.available())
    {
        if (millis() - timeout > 3000)
        {
            client.stop();
            return;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    String response = "";
    while (client.available())
    {
        char c = client.read();
        response += c;
        if (response.length() > 4096)
            break;
    }

    client.stop();

    if (response.length() > 0)
    {
        int jsonBeginAt = response.indexOf("{");
        if (jsonBeginAt != -1)
        {
            int jsonEndAt = response.lastIndexOf("}");
            if (jsonEndAt > jsonBeginAt)
            {
                processMessage(response.substring(jsonBeginAt, jsonEndAt + 1));
            }
        }
    }
}

void processMessage(const String &jsonData)
{

    StaticJsonDocument<2048> doc; // 增加到2048字节

    DeserializationError error = deserializeJson(doc, jsonData);
    if (error)
    {
        Serial.println(error.c_str());
        return;
    }

    // 检查JSON结构
    if (!doc.containsKey("results"))
    {
        return;
    }

    JsonArray results = doc["results"];
    if (results.size() == 0)
    {
        return;
    }

    JsonObject results_0 = results[0];
    if (!results_0.containsKey("now"))
    {
        return;
    }

    JsonObject now = results_0["now"];

    // 直接使用字符串转整数,更可靠
    if (now.containsKey("code"))
    {
        day1.tianqitubiao = now["code"].as<int>();
    }
    if (now.containsKey("temperature"))
    {
        day1.temperature = now["temperature"].as<int>();
    }
    if (now.containsKey("pressure"))
    {
        day1.pressure = now["pressure"].as<int>();
    }
    if (now.containsKey("humidity"))
    {
        day1.shidu = now["humidity"].as<int>();
    }
    if (now.containsKey("wind_direction_degree"))
    {
        day1.fengjiaodu = now["wind_direction_degree"].as<int>();
    }
    if (now.containsKey("wind_scale"))
    {
        const char *scale = now["wind_scale"];
        day1.dengji = atoi(scale); // 风级可能是字符串
    }
}

void oledClockDisplay()
{
    years = year();
    months = month();
    days = day();
    hours = hour();
    minutes = minute();
    seconds = second();
    weekdays = weekday();
    tens = (years / 10) % 10;
    ones = years % 10;
}

const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

time_t getNtpTime()
{
    IPAddress ntpServerIP;

    while (Udp.parsePacket() > 0)
        ;
    WiFi.hostByName(ntpServerName, ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            isNTPConnected = true;
            Udp.read(packetBuffer, NTP_PACKET_SIZE);
            unsigned long secsSince1900;
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    isNTPConnected = false;
    return 0;
}

void sendNTPpacket(IPAddress &address)
{
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    Udp.beginPacket(address, 123);
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

String getStoryFromAPI()
{
    HTTPClient http;
    String url = "https://apis.tianapi.com/story/index?key=5352af6fdf1c237d0e465e72bb0d1fa6";
    http.begin(url);
    http.setTimeout(10000);  // 增加超时时间
    int httpCode = http.GET();

    if (httpCode == 200)
    {
        String payload = http.getString();
        
        // 增加JSON缓冲区到4096字节
        DynamicJsonDocument doc(4096);  // 改用动态分配，避免栈溢出
        
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            http.end();
            return "故事解析失败";
        }
        
        if (doc["result"]["list"].size() > 0)
        {
            String title = doc["result"]["list"][0]["title"].as<String>();
            String content = doc["result"]["list"][0]["content"].as<String>();
            
            http.end();
            
            // 限制总长度在400字以内
            String result = title + "。 " + content;
            if (result.length() > 400) {
                result = result.substring(0, 400);
            }
            return result;
        }
        else
        {
            http.end();
            return "未获取到故事内容";
        }
    }
    else
    {
        http.end();
        return "故事API请求失败";
    }
}

String getMorningFromAPI()
{
    HTTPClient http;
    String url = "https://apis.tianapi.com/zaoan/index?key=5352af6fdf1c237d0e465e72bb0d1fa6";
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200)
    {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            return "早安解析失败";
        }
        // 直接取 result.content
        if (doc["result"].containsKey("content"))
        {
            String content = doc["result"]["content"].as<String>();
            return content;
        }
        else
        {
            return "未获取到早安内容";
        }
    }
    else
    {
        return "早安API请求失败";
    }
}

String getJokeFromAPI()
{
    HTTPClient http;
    String url = "https://apis.tianapi.com/joke/index?key=5352af6fdf1c237d0e465e72bb0d1fa6&num=1";
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200)
    {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            return "笑话解析失败";
        }
        if (doc["result"]["list"].size() > 0)
        {
            String content = doc["result"]["list"][0]["content"].as<String>();
            return content;
        }
        else
        {
            return "未获取到笑话内容";
        }
    }
    else
    {
        return "笑话API请求失败";
    }
}
