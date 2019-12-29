DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_AddCache //
CREATE PROCEDURE Alita.usp_AddCache (
    IN  _link    VARCHAR(4000)  CHARSET utf8,
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