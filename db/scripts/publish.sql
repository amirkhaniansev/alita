-- create database Alita
CREATE DATABASE Alita;

-- create table Cache
CREATE TABLE Alita.Cache (
    ID              INT             NOT NULL AUTO_INCREMENT,
    Created         DATETIME        NOT NULL,
    Modified        DATETIME        NOT NULL,
    ProcessState    TINYINT         NOT NULL,
    Link            VARCHAR(2000)   CHARSET utf8,
    Content         LONGTEXT        CHARSET utf8,

    CONSTRAINT PK_CACHE_ID      PRIMARY KEY (ID)
);
SHOW WARNINGS;

-- create table Word
CREATE TABLE Alita.Word (
    ID              INT             NOT NULL AUTO_INCREMENT,
    Created         DATETIME        NOT NULL,
    Content         VARCHAR(2000)   CHARSET utf8,

    CONSTRAINT PK_WORD_ID      PRIMARY KEY (ID)
);
SHOW WARNINGS;

-- create table Index
CREATE TABLE Alita.Index (
    ID              INT             NOT NULL AUTO_INCREMENT,
    WordID          INT             NOT NULL,
    LinkID          INT             NOT NULL,
    Created         DATETIME        NOT NULL,
    Modified        DATETIME        NOT NULL,
    Frequency       INT             NOT NULL,

    CONSTRAINT PK_INDEX_ID         PRIMARY KEY (ID),
    CONSTRAINT FK_INDEX_WORD_ID    FOREIGN KEY (WordID) REFERENCES Alita.Word(ID),
    CONSTRAINT FK_INDEX_LINK_ID    FOREIGN KEY (LinkID) REFERENCES Alita.Cache(ID)
);
SHOW WARNINGS;

-- create procedure usp_AddCache
DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_AddCache //
CREATE PROCEDURE Alita.usp_AddCache (
    IN  _link    VARCHAR(2000)  CHARSET utf8,
    IN  _content LONGTEXT       CHARSET utf8,
    OUT _linkId  INT
)
PROC_START : BEGIN
    SET @id := NULL;
    SELECT @id := Id FROM Alita.Cache WHERE Link = _link;
    SET _linkId := @id;
    
    IF @id IS NULL THEN 
        INSERT INTO Alita.Cache VALUES(0, NOW(), NOW(), 0, _link, _content);
        SELECT @id := Id FROM Alita.Cache WHERE Link = _link;
        SET _linkId := @id;
        LEAVE PROC_START;
    END IF;

    UPDATE Alita.Cache 
    SET Content = _content, 
        Modified = NOW(), 
        ProcessState = 0 
        WHERE Id = @id AND ProcessState = 2;
END //
DELIMITER ;
SHOW WARNINGS;

-- create procedure usp_AddIndex
DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_AddIndex //
CREATE PROCEDURE Alita.usp_AddIndex (
    IN _word        VARCHAR(2000) CHARSET utf8,
    IN _link        VARCHAR(2000) CHARSET utf8,
    IN _frequency   INT
)
PROC_START : BEGIN
    SET @wordId := NULL;
    SELECT @wordId := Id FROM Alita.Word WHERE Content = _word;
    IF @wordId IS NULL THEN
        INSERT INTO Alita.Word VALUES (0, NOW(), _word);
        SELECT @wordId := Id FROM Alita.Word WHERE Content = _word;
    END IF;

    SET @linkId := NULL;
    SELECT @linkId := Id FROM Alita.Cache WHERE Link = _link;
    IF @linkId IS NULL THEN
        LEAVE PROC_START;
    END IF;

    SET @indexId := NULL;
    SELECT @indexId := Id FROM Alita.Index WHERE WordID = @wordId AND LinkID = @linkId;
    IF @indexId IS NULL THEN
        INSERT INTO Alita.Index VALUES (0, @wordId, @linkId, NOW(), NOW(), _frequency);
        LEAVE PROC_START;
    END IF;

    UPDATE Alita.Index 
    SET Modified = NOW(),
        Frequency = _frequency
        WHERE WordID = @wordId AND LinkID = @linkId;
END //
DELIMITER ;
SHOW WARNINGS;

-- create procedure usp_GetCacheById
DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_GetCacheById //
CREATE PROCEDURE Alita.usp_GetCacheById (
    IN cacheId INT
)
BEGIN
    SELECT * FROM Alita.Cache WHERE Id = cacheId;
END //
DELIMITER ;
SHOW WARNINGS;

-- create procedure usp_Search
DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_Search //
CREATE PROCEDURE Alita.usp_Search (
    IN _word    VARCHAR(2000) CHARSET utf8,
    IN _lastId  INT
)
BEGIN
    SELECT  i.Id        AS IndexId,
            w.Content   AS Word,
            i.Modified  AS LastIndexed,
            c.Link      AS Page,
            i.Frequency AS Frequency
        FROM Alita.Word w
        INNER JOIN Alita.Index i ON w.Id = i.WordId 
        INNER JOIN Alita.Cache c ON i.LinkId = c.Id
        WHERE INSTR(w.Content, _word) != 0  AND
              i.Id > _lastId 
        ORDER BY i.Frequency DESC, 
                 i.Modified  DESC,
                 i.Id ASC
        LIMIT 0, 10;
END //
DELIMITER ;
SHOW WARNINGS;

-- create procedure usp_SetCacheState
DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_SetCacheState //
CREATE PROCEDURE Alita.usp_SetCacheState (
    IN _linkId  INT,
    IN _state   TINYINT
)
BEGIN
    UPDATE Alita.Cache SET ProcessState = _state WHERE Id = _linkId AND ProcessState != _state;
END //
DELIMITER ;
SHOW WARNINGS;