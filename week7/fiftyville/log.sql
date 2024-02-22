
--Look at scrime scene report
.schema crime_scene_reports

SELECT * FROM crime_scene_reports
WHERE month  = 7 AND day = 28 AND year = 2023;

--Humphrey Street Bakery - 10:15am - 3 witness - interviewed

.schema interviews
SELECT * FROM interviews
WHERE month  = 7 AND day = 28 AND year = 2023 AND transcript LIKE "%bakery%";

--Thief left in car with in 10 minutes - Camera Bakery parking lot
--Face known - Withdew Money ATM - Leggett Street - 28-07-2023 morning
--Phone call - earliest flight at 29-07-2023 - out fiftyville

-------------------------Bakery Security logs ----------------------------------
--looking at card enter and exit betwen 9 and 11
.schema bakery_security_logs
SELECT * FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND year = 2023 AND hour >= 9 AND hour < 11 AND minute < 35 AND activity =  "exit";

-- car 1;  G412CB7

--ATM_transactions
-- looking at ATM transactions withraw at leggett street because of the interview report
.schema atm_transactions
SELECT * FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2023 AND transaction_type = "withdraw" AND atm_location = "Leggett Street";

--Phone call logs
-- looking at call logs and people.
.schema phone_calls
SELECT * FROM phone_calls
WHERE  month = 7 AND day = 28 AND year = 2023;

-- look at people
.schema people

--it has name, phonenumber, license_plate.
-- i am thinking of printing the name of people whose numberplate has exited the bakery between 9 and 11
-- who have made a call.
 SELECT DISTINCT p.name, p.license_plate, p.phone_number
 FROM people p
 JOIN bakery_security_logs en
    ON p.license_plate = en.license_plate
 JOIN bakery_security_logs ex
    ON en.license_plate = ex.license_plate
 JOIN phone_calls pc
    ON p.phone_number = pc.caller
    AND ex.activity = "exit"
    AND en.year = 2023
    AND en.month = 7
    AND en.day= 28
    AND en.hour >= 9
    AND en.hour < 11
    AND en.minute < 35
    AND pc.duration <= 60;

--Because in the interview. Interviewey saw phone call. Some i only searched for phone caller not reciever. In my openion
-- the thief would call after success or failer.
-- Now looking for name of people who have made a phone call, and withdrew money from the atm in the morning

.schema bank_accounts

 SELECT DISTINCT p.name, p.phone_number, atm.transaction_type FROM people p
 JOIN bank_accounts ba ON p.id = ba.person_id
 JOIN atm_transactions atm ON ba.account_number  = atm.account_number
JOIN bakery_security_logs en
    ON p.license_plate = en.license_plate
 JOIN bakery_security_logs ex
    ON en.license_plate = ex.license_plate
 JOIN phone_calls pc
    ON p.phone_number = pc.caller
 WHERE ex.activity = "exit"
    AND en.year = 2023
    AND en.month = 7
    AND en.day= 28
    AND en.hour >= 9
    AND en.hour < 11
    AND en.minute < 35
    AND atm.transaction_type = "withdraw"
    AND atm.atm_location = "Leggett Street"
    AND pc.duration <= 60;

-- now we have narrowed the people list down to 2. Now need to look at flight logs
 SELECT * FROM airports;

 --fiftyville airport code is CSF
.schema flights

--looking at earlies flight out of fiftyville 29-07-2023
SELECT * FROM flights f
JOIN airports a ON f.origin_airport_id = a.id
WHERE a.id =
(
   SELECT id FROM airports
   WHERE abbreviation = "CSF"
)
AND f.year = 2023
AND f.month = 7
AND f.day = 29;

-- earlies flight is at 8:20
-- now we have flight time we gonna look at passanger list
.schema passengers

SELECT * FROM passengers p
JOIN flights f ON p.flight_id = f.id
JOIN airports a ON f.origin_airport_id = a.id
WHERE f.id =
(
   SELECT id FROM flights
   WHERE a.id =
   (
      SELECT id FROM airports
      WHERE abbreviation = "CSF"
   )
   AND flights.year = 2023
   AND flights.month = 7
   AND flights.day = 29
   AND flights.hour = 8
   AND flights.minute = 20
);
-- We got the passanger list. Now need to check if the suspected people are in the list

SELECT DISTINCT p.name, p.phone_number, atm.transaction_type FROM people p
JOIN bank_accounts ba ON p.id = ba.person_id
JOIN atm_transactions atm ON ba.account_number  = atm.account_number
JOIN bakery_security_logs en ON p.license_plate = en.license_plate
JOIN bakery_security_logs ex ON en.license_plate = ex.license_plate
JOIN phone_calls pc ON p.phone_number = pc.caller
JOIN passengers pa ON p.passport_number = pa.passport_number
JOIN flights f ON pa.flight_id = f.id
JOIN airports a ON f.origin_airport_id = a.id
WHERE f.id =
(
   SELECT id FROM flights
   WHERE a.id =
   (
      SELECT id FROM airports
      WHERE abbreviation = "CSF"
   )
   AND flights.year = 2023
   AND flights.month = 7
   AND flights.day = 29
   AND flights.hour = 8
   AND flights.minute = 20
)
   AND ex.activity = "exit"
   AND en.year = 2023
   AND en.month = 7
   AND en.day= 28
   AND en.hour >= 9
   AND en.hour < 11
   AND en.minute < 35
   AND atm.transaction_type = "withdraw"
   AND atm.atm_location = "Leggett Street"
   AND pc.duration <= 60;

-- Now we have found the name Bruce
-- Now need to find the destination

SELECT DISTINCT p.name, p.phone_number, atm.transaction_type, a.abbreviation, a.city as origin, ai.city as destination FROM people p
JOIN bank_accounts ba ON p.id = ba.person_id
JOIN atm_transactions atm ON ba.account_number  = atm.account_number
JOIN bakery_security_logs en ON p.license_plate = en.license_plate
JOIN bakery_security_logs ex ON en.license_plate = ex.license_plate
JOIN phone_calls pc ON p.phone_number = pc.caller
JOIN passengers pa ON p.passport_number = pa.passport_number
JOIN flights f ON pa.flight_id = f.id
JOIN airports a ON f.origin_airport_id = a.id
JOIN airports ai ON f.destination_airport_id = ai.id
WHERE f.id =
(
   SELECT id FROM flights
   WHERE a.id =
   (
      SELECT id FROM airports
      WHERE abbreviation = "CSF"
   )
   AND flights.year = 2023
   AND flights.month = 7
   AND flights.day = 29
   AND flights.hour = 8
   AND flights.minute = 20
)
   AND ex.activity = "exit"
   AND en.year = 2023
   AND en.month = 7
   AND en.day= 28
   AND en.hour >= 9
   AND en.hour < 11
   AND en.minute < 35
   AND atm.transaction_type = "withdraw"
   AND atm.atm_location = "Leggett Street"
   AND pc.duration <= 60;

-- We have found the destination now need to find his ACCOMPLICE
-- we need to see who he called

SELECT p.name as caller , ph.name as receiver, pc.day, pc.month, pc.year FROM phone_calls pc
JOIN people p ON pc.caller = p.phone_number
JOIN people ph ON pc.receiver = ph.phone_number
WHERE p.phone_number = "(367) 555-5533"
AND pc.day = 28
AND pc.duration <= 60;

-- By checking number and duration and date we find the name Robin
