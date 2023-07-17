SELECT SUM(rating) / COUNT(rating) FROM ratings JOIN movies ON ratings.movie_id = movies.id WHERE year = "2012";
