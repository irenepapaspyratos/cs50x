-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the right crime report
SELECT id, description FROM crime_scene_reports WHERE day = "28" AND month = "7" AND year = "2020" AND street = "Chamberlin Street";
-- Result=  id: 295. time of crime: 10:15am. 3 interviews (same day), each mentioning the courthouse.

-- Check relevant interviews
SELECT id, name, transcript  FROM interviews WHERE day = "28" AND month = "7" AND year = "2020" AND transcript LIKE "%courthouse%";
-- Result=  161, Ruth: 10:15-10.25 thief left with car from parking lot.
--          162, Eugene: thief seen same morning withdrawing at ATM Fifer Street
--          163, Raymond: thief made call (<1 min) while leaving courthouse -> accomplice should purchase ticket for earliest flight out of town next day

-- Check how leaving the parking lot is called
SELECT activity FROM courthouse_security_logs;
-- Result=  exit

-- Check how duration of phone_calls is documented
SELECT duration FROM phone_calls;
-- Result=  min duration has 2 digits -> must be counted in seconds

-- Check (for the day of the theft) if...
-- withdrawing person at relevant ATM in the morning
-- = person leaving parking lot at relevant time
-- = person who made a call with specific duration
-- and get the id, name, call receiver
SELECT people.id, people.name, phone_calls.receiver  FROM people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE courthouse_security_logs.day = "28" AND atm_transactions.day = "28" AND phone_calls.day = "28"
AND courthouse_security_logs.month = "7" AND atm_transactions.month = "7" AND phone_calls.month = "7"
AND courthouse_security_logs.year = "2020" AND atm_transactions.year = "2020" AND phone_calls.year = "2020"
AND courthouse_security_logs.activity LIKE "exit" AND courthouse_security_logs.minute > 15 AND courthouse_security_logs.minute < 25
AND atm_transactions.transaction_type = "withdraw" AND atm_transactions.atm_location = "Fifer Street"
AND phone_calls.duration < 60;
-- Result=  686048, Ernest, (375) 555-8161 and 514354, Russel, (725) 555-3243

-- Find id of first flight out of town next morning
SELECT flights.id FROM flights
JOIN airports ON origin_airport_id = airports.id
WHERE day = "29" AND month = "7" AND year = "2020" AND airports.city LIKE "%fiftyville%" ORDER BY hour ASC, minute ASC LIMIT 1;
-- Result=  36

-- Check, if for one of them a flight is booked next morning
SELECT name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id = "36" AND name = "Ernest" OR name = "Russel";
-- Result=  Ernest. This should be the thief.

-- Find destination of flight
SELECT * FROM airports JOIN flights ON airports.id = flights.destination_airport_id WHERE flights.id = "36";
-- Result=  4 | LHR | Heathrow Airport | London | 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20

-- Find accomplice through phone number (receiver of Ernest's call)
SELECT name FROM people WHERE people.phone_number = "(375) 555-8161";
-- Result=  Berthold
