CREATETABLEIFNOTEXISTSobjects(
    idINTAUTO_INCREMENTPRIMARYKEY,
    serialVARCHAR(64)NOTNULLUNIQUE,
    nameVARCHAR(255)NOTNULL
);
CREATETABLEIFNOTEXISTSrelationships(
parent_idINT,
child_idINT,
PRIMARYKEY(
    parent_id,
    child_id
),
FOREIGNKEY(
    parent_id
)REFERENCESobjects(id)
    ON DELETECASCADEFOREIGNKEY(
        child_id
    )REFERENCESobjects(id)
        ON DELETECASCADE
);
-- Add root object
INSERT INTO objects(
serial,
name
)
VALUES(
'ROOT001',
'Root Item'
);
-- Insert new objects
INSERT INTO objects(
serial,
name
)
VALUES(
'ITEM002',
'Item A'
),
(
'ITEM003',
'Item B'
),
(
'ITEM004',
'Item C'
),
(
'ITEM005',
'Item D'
);
-- Establish parent-child relationships
INSERT INTO relationships(
parent_id,
child_id
)
VALUES(
(
SELECT
id
FROM
objects
WHERE
serial = 'ROOT001'
),
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM002'
)
),
(
(
SELECT
id
FROM
objects
WHERE
serial = 'ROOT001'
),
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM003'
)
),
(
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM002'
),
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM004'
)
),
(
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM003'
),
(
SELECT
id
FROM
objects
WHERE
serial = 'ITEM005'
)
);
