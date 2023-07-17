-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the right crime report
SELECT id, description FROM crime_scene_reports WHERE day = "28" AND month = "7" AND year = "2021" AND street = "Humphrey Street";
-- Result=  id: 295. time of crime: 10:15am. 3 interviews (same day), each mentioning the bakery.

-- Check relevant interviews
SELECT id, name, transcript  FROM interviews WHERE day = "28" AND month = "7" AND year = "2021" AND transcript LIKE "%bakery%";
-- Result=  161, Ruth: 10:15-10.25 thief left with car from parking lot.
--          162, Eugene: thief seen same morning withdrawing at ATM Leggett Street
--          163, Raymond: thief made call (<1 min) while leaving bakery -> accomplice should purchase ticket for earliest flight out of town next day

-- Check how leaving the parking lot is called
SELECT activity FROM bakery_security_logs;
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
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE bakery_security_logs.day = "28" AND atm_transactions.day = "28" AND phone_calls.day = "28"
AND bakery_security_logs.month = "7" AND atm_transactions.month = "7" AND phone_calls.month = "7"
AND bakery_security_logs.year = "2021" AND atm_transactions.year = "2021" AND phone_calls.year = "2021"
AND bakery_security_logs.activity LIKE "exit" AND bakery_security_logs.minute > 15 AND bakery_security_logs.minute < 25
AND atm_transactions.transaction_type = "withdraw" AND atm_transactions.atm_location = "Leggett Street"
AND phone_calls.duration < 60;
-- Result=  686048, Bruce, (375) 555-8161 and 514354, Diana, (725) 555-3243

-- Find id of first flight out of town next morning
SELECT flights.id FROM flights
JOIN airports ON origin_airport_id = airports.id
WHERE day = "29" AND month = "7" AND year = "2021" AND airports.city LIKE "%fiftyville%" ORDER BY hour ASC, minute ASC LIMIT 1;
-- Result=  36

-- Check, if for one of them a flight is booked next morning (taking into account, that several Dianas and/or Bruces might exist)
SELECT name, people.id FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE (flights.id = "36") AND ((name = "Bruce" AND people.id = "686048") OR (name = "Diana" AND people.id = "514354"));
-- Result=  Bruce. This should be the thief.

-- Find destination of flight
SELECT * FROM airports JOIN flights ON airports.id = flights.destination_airport_id WHERE flights.id = "36";
-- Result=  4 | LGA | LaGuardia Airport | New York City | 36 | 8 | 4 | 2021 | 7 | 29 | 8 | 20

-- Find accomplice through phone number (receiver of Bruce's call)
SELECT name FROM people WHERE people.phone_number = "(375) 555-8161";
-- Result=  Robin
