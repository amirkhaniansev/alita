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