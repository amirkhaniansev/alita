### Database Scheme

#### Tables

* ##### Cache

ID | INT PRIMARY KEY AUTO_INCREMENT           
--- | --- 
Created | DATETIME
Modified | DATETIME
ProcessState | TINYINT
Link | VARCHAR(2000)
Content | VARCHAR(4000)

* ##### Word

ID | INT PRIMARY KEY AUTO_INCREMENT           
--- | --- 
Created | DATETIME
Content | VARCHAR(2000)

 * ##### Index

ID | INT PRIMARY KEY AUTO_INCREMENT           
--- | --- 
WordID | INT FOREIGN KEY TO Word
LinkID | INT FOREIGN KEY TO Cache
Created | DATETIME
Modified | DATETIME
Frequency | INT

#### Stored Procedures

 * ##### usp_AddCache(_link VARCHAR(2000), _content LONGTEXT, out _linkId)
 * ##### usp_AddIndex(_word VARCHAR(2000), _link VARCHAR(2000), _frequency INT)
 * ##### usp_SetCacheState(_linkId INT, _state TINYINT)
 * ##### usp_GetCacheById(_cacheId INT)
 * ##### usp_Search(_word VARCHAR(2000), _lastId INT)
