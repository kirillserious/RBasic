#
# Считает сумму элементов массива
#
array.sum <- function (array) {
    a <- 0
    for (i in array) {
        a <- a + i
    }

    a
}

#
# Факториал числа
#
factorial <- function (number) {
    # Создали вектор-счётчик
    counter <- NULL;
    counter [number - 1] <- NULL;
    
    # Создали вектор-результат
    result <- 1;
    iter   <- 2;
    
    for (i in counter) {
        result <- result * iter
        iter   <- iter + 1
    }

    result
}

#
# Cреднее аримметическое массива
#
array.average <- function (array) {
    sum <- array.sum(array)

    sum / length(array)
}

#
# Возведение массива в степень 
#
array.pow <- function (array, pow = 1) {
    # Создали вектор-счётчик
    counter <- NULL;
    counter [pow - 1] <- NULL;

    for (i in array) {
        first <- i;
        for (j in counter) {
            i <- i * first;
        }
    }

    array
} 

#
# Мой if
#
my.if <- function (condition, exp1, exp2 = NULL) {
    sel <- c(exp1, exp2)
    bcond <- condition & TRUE

    sel[c(bcond, !bcond)]
}

#
# Модуль числа
#
abs <- function (number) {
    my.if(number >= 0, number, 0 - number)
}

#
# Модуль для массива
#
array.abs <- function (array) {
    for (i in array) {
        i <- abs(i)
    }
    array
}

#
# Квадратный корень
#
sqrt <- function (number) {
    squareRoot <- number / 2
    t <- squareRoot

    cnt <- NULL
    cnt[10000] <- NULL
    for (i in cnt) {
        t <- squareRoot
        squareRoot <- (t + (number / t)) / 2
    }

    squareRoot
}

array.erase <- function (array, from, to = 0) {
    to <- my.if(to == 0, from + 1, to)
    i <- 1
    j <- 1
    tmp <- NULL
    for (iterator in array) {
        tmp[j] <- my.if ((i < from) | (i >= to), i, 0);
        j <- my.if ((i < from) | (i >= to), j + 1, j);
        i <- i + 1;
    } 
    tmp
}

string.pow <- function (string, pow = 1) {
    # Создали вектор-счётчик
    counter <- NULL;
    counter [pow - 1] <- NULL;
    for (i in string) {
        first <- i;
        for (j in counter) {
            i <- i + first;
        }
    }

    string
} 