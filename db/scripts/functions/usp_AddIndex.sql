DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_AddIndex //
CREATE PROCEDURE Alita.usp_AddIndex (
    IN _word        VARCHAR(4000) CHARSET utf8,
    IN _link        VARCHAR(4000) CHARSET utf8,
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