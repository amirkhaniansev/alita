DELIMITER //
DROP PROCEDURE IF EXISTS Alita.usp_GetCacheById //
CREATE PROCEDURE Alita.usp_GetCacheById (
    IN cacheId INT
)
BEGIN
    SELECT * FROM Alita.Cache WHERE Id = cacheId;
END //
DELIMITER ;