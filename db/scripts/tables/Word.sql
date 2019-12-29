CREATE TABLE Alita.Word (
    ID              INT             NOT NULL AUTO_INCREMENT,
    Created         DATETIME        NOT NULL,
    Content         VARCHAR(4000)   CHARSET utf8,

    CONSTRAINT WORD_ID PRIMARY KEY (ID)    
);
SHOW WARNINGS;