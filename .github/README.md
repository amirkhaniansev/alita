# ALITA
![alt Alita is a hero in an American sci-fi movie.](https://repository-images.githubusercontent.com/225002165/4fae2100-1fa8-11ea-97ac-fe4735d0e4ae)
* Simple search engine with basic functionality. 
* NOTE that this project is done for educational and scientific purposes, thus it may not meet industrial and production standards.

The problem is offered and described by [Vahram Martirosyan, Ph.D](https://github.com/vmartirosyan).

The ecosystem of this seach engine consists of a few components:
* ___Crawler___
* ___Link Queue___
* ___Pub / Sub System___
* ___Indexing Service___
* ___Ranking Service___
* ___Storage___
* ___Search Programmatic API___
* ___Search API Host___
* ___Search API UI___
* ___User Services___

#### Crawler
  * Crawler surfs all the internet parsing the content of the websites and collecting the hyperlinks in the link queue for further crawling. Crawler starts its work from the _initial list of most informative links_ stored in the link queue. 
  Below are the steps which crawler should do :
     1. Pop a link from link queue.
     2. Crawl the page.
     3. Parse the HTML content of the page.
     4. Push the hyperlinks to the link queue.
     5. Publish the content of parsed pages with the defined topic.

#### Link Queue
  * Link Queue will probably be customly designed multiple push / pop queue framework suitable for this purpose. 
  
#### Pub / Sub System
  * Pub / Sub System is a library providing convenient abstractions over RabbitMQ. This is module is used by the _Crawler_ and _Indexing Service_. Crawlers publish the parsed content with defined topic, whereas Indexing Service subsribes to this topic to be able to get updates. In the future _Ranking Service_ can also be subscribed.

#### Indexing Service
  * The primary goal of Indexing Service is to index the parsed content to improve search speed.
  Operations which this service should do:
    1. Subcribe to the defined topic.
    2. Add the results to storage.
  
#### Ranking Service
  * Ranking Service ranks the results, domains and links to improve the quality and relevancy of the search results. This module is a relatively active component.
  
#### Storage
  * Storage is a quite passive component. It is a separate process which provides API for our MySQL database.
  API that Storage componenet exposes:
    1. AddCache - add page content
    2. Search
  
#### Search Programmatic API
  * This one is a library to execute search queries.
  
#### Search API Host
  *  This is a service which exposes public suitable API for programmatic API. 

#### Libraries and Services we use
  *  [RabbitMQ](https://github.com/rabbitmq/rabbitmq-server)
  *  [RabbitMQ C Client](https://github.com/alanxz/rabbitmq-c)
  *  [RabbitMQ C++ Client](https://github.com/alanxz/SimpleAmqpClient)
  *  [MySql](https://github.com/mysql/mysql-server)
  *  [MySql C++ Connector]( https://github.com/mysql/mysql-connector-cpp])
  *  [HTTP(S) C++ Client](https://github.com/yhirose/cpp-httplib)
  *  [HTML Parser](https://github.com/lexborisov/myhtml)
  *  [CURL](https://curl.haxx.se/libcurl/)
  
#### TODOs
  * Aggressive Optimizations
  * Desktop / Web UI
  * Monitoring System
  * Discovery Service
