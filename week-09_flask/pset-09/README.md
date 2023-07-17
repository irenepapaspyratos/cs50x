# C$50 Finance

https://cs50.harvard.edu/x/2023/psets/9/finance/

Implement a website via which users can “buy” and “sell” stocks.

Note, that this project won't work without an api key from `iexcloud.io`.

## My Additions

- several templates

- 2 Tables: 

    - actions
        
        | id | action |
        | :- | :- |
        | 1  | buy    |
        | 2  | sell   |
        | 3  | initial|
        | 4  | deposit|
        
        ```
        with
        id INTEGER,
        action TEXT NOT NULL,
        PRIMARY KEY(id)
        and
        UNIQUE INDEX action ON actions(action)
        ```

    - transactions_utc
        
        | id | usser_id | symbol | date | time | action_id | shares | price |
        | - | - | - | - | - | - | - | - |
        
        ```
        with
        id INTEGER, 
        user_id INTEGER, 
        symbol TEXT NOT NULL, 
        date DATE NOT NULL, 
        time TEXT NOT NULL, 
        action_id INTEGER, 
        shares INTEGER NOT NULL, 
        price FLOAT NOT NULL, 
        PRIMARY KEY(id), 
        FOREIGN KEY (user_id) REFERENCES users(id), 
        FOREIGN KEY (action_id) REFERENCES actions(id)
        and
        INDEX user_id ON transactions_utc (user_id), 
        INDEX symbol ON transactions_utc (symbol),
        INDEX action_id ON transactions_utc (action_id)
        ```

- A function, i.e. route, to allow users to add additional cash to their account.

- Errorhandler
