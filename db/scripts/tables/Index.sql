CREATE TABLE Alita.Index (
    ID              INT             NOT NULL AUTO_INCREMENT,
    WordID          INT             NOT NULL,
    LinkID          INT             NOT NULL,
    Created         DATETIME        NOT NULL,
    Modified        DATETIME        NOT NULL,
    Frequency       INT             NOT NULL,

    CONSTRAINT PK_INDEX_ID         PRIMARY KEY (ID),
    CONSTRAINT FK_INDEX_WORD_ID    FOREIGN KEY (WordID) REFERENCES Alita.Word(ID),
    CONSTRAINT FK_INDEX_LINK_ID    FOREIGN KEY (LinkID) REFERENCES Alita.Cache(ID),
    CONSTRAINT UQ_CACHE_LWID       UNIQUE (WordID, LinkID)
);
SHOW WARNINGS;