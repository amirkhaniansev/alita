CREATE TABLE Alita.Word (
    ID              INT             NOT NULL AUTO_INCREMENT,
    Created         DATETIME        NOT NULL,
    Content         VARCHAR(2000)   CHARSET utf8,

    CONSTRAINT PK_WORD_ID      PRIMARY KEY (ID)
);
SHOW WARNINGS;