CREATE TABLE Alita.Index (
    ID              INT             NOT NULL AUTO_INCREMENT,
    WordID          INT             NOT NULL,
    LinkID          INT             NOT NULL,
    Created         DATETIME        NOT NULL,
    Modified        DATETIME        NOT NULL,
    Frequency       INT             NOT NULL,

    CONSTRAINT INDEX_ID         PRIMARY KEY (ID),
    CONSTRAINT INDEX_WORD_ID    FOREIGN KEY (WordID) REFERENCES Alita.Word(ID),
    CONSTRAINT INDEX_LINK_ID    FOREIGN KEY (LinkID) REFERENCES Alita.Cache(ID) 
);
SHOW WARNINGS;