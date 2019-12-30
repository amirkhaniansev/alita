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
        WHERE (w.Content = _word OR LOCATE(_word, w.Content) != 0) AND
              i.Id > _lastId 
        ORDER BY i.Frequency DESC, 
                 i.Modified  DESC,
                 i.Id ASC
        LIMIT 0, 10;
END //
DELIMITER ;
SHOW WARNINGS;