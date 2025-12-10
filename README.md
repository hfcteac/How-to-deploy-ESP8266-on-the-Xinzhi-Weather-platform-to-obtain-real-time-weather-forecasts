# How-to-deploy-ESP8266-on-the-Xinzhi-Weather-platform-to-obtain-real-time-weather-forecasts
关于ESP8266/ESP32如何部署心知天气平台获取实时天气预报

原创作者：宋楠    
职位：嵌入式工程师   
仅供学习，禁止商用，谢谢配合   


![C](https://img.shields.io/badge/C-A8B9CC?style=flat-square&logo=c&logoColor=black)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat-square&logo=arduino&logoColor=white)    

本项目包含两个版本的代码，分别为esp8266和esp32版本，二者皆可使用。   
esp8266使用的是单线程阻塞查询法，在查询天气的时候可能会无输出1-3s，单核暂时无法优化，不影响使用     
esp32使用双核交替运行非阻塞查询法，无需延迟等待。    
由于是比赛作品可能会包含一些其他的部分，属于选配，懒得删除了，敬请谅解。如果做毕业设计需要的话也可以使用，没问题的

作者邮箱：song1234588@163.com（主），hafeizhou999@gmail.com（不常上）
QQ:3024026153


Original Author: Song Nan   
Position: Embedded Engineer    
For learning purposes only, commercial use prohibited. Thank you for your cooperation.   

This project contains two versions of code: one for ESP8266 and one for ESP32, both of which are usable.     
The ESP8266 version uses a single-threaded blocking query method, which may result in 1-3 seconds of no output when querying weather. This cannot be optimized for single-core processors at the moment, but it does not affect usability.     
The ESP32 version uses a dual-core alternating non-blocking query method, eliminating any delays.      
Since this is a competition entry, it may contain some other optional components, which I haven't bothered to remove. Please understand. It can also be used for graduation projects without issue.      
Author's email: song1234588@163.com (main), hafeizhou999@gmail.com (not frequently online)     
QQ: 3024026153     

2025年12月10日