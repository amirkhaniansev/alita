CREATE TABLE SearchEngine.Cache(
    ID          INT             NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Created     DATETIME        NOT NULL,
    Modified    DATETIME        NOT NULL,
    Link        VARCHAR(4000)   CHARACTER SET utf8 COLLATE utf8_unicode_ci,
    Content     VARCHAR(4000)   CHARACTER SET utf8 COLLATE utf8_unicode_ci
);