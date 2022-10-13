package ba.etf.rma22.projekat.data

import ba.etf.rma22.projekat.data.models.Anketa
import java.util.*

fun date(dan: Int, mjesec:Int, godina: Int): Date {
    var cal: Calendar= Calendar.getInstance()
    cal.set(godina,mjesec,dan)
    return cal.time
}

fun ankete(): List<Anketa> {
    return listOf(
        Anketa(0,"Anketa 1",date(10,2,2022),date(22,8,2022),25),
        Anketa(1,"Anketa 2",date(1,2,2022),date(20,2,2022),25),
        Anketa(2,"Anketa 3",date(1,2,2022),date(20,2,2022),25),
        Anketa(3,"Anketa 1",date(1,2,2022),date(10,7,2022),25),
        Anketa(4,"Anketa 2",date(1,2,2022),date(10,7,2022),25),
        Anketa(5,"Anketa 3",date(1,2,2022),date(10,7,2022),25),
        Anketa(6,"Anketa 1",date(10,7,2022),date(10,9,2022),25),
        Anketa(7,"Anketa 2",date(15,2,2022),date(20,7,2022),35),
        Anketa(8,"Anketa 1",date(10,7,2022),date(10,9,2022),30),
        Anketa(9,"Anketa 2",date(10,7,2022),date(10,9,2022),25),
        Anketa(10,"Anketa 1",date(10,7,2022),date(10,9,2022),25),
        Anketa(11,"Anketa 2",date(10,1,2022),date(10,2,2022),25),
        Anketa(12,"Anketa 1",date(10,1,2022),date(10,2,2022),25),
        Anketa(13,"Anketa 2",date(1,2,2022),date(10,7,2022),45),
        Anketa(14,"Anketa 3",date(10,1,2022),date(10,7,2022),25),
        Anketa(15,"Anketa 1",date(10,7,2022),date(10,9,2022),25),
        Anketa(16,"Anketa 2",date(10,7,2022),date(10,9,2022),25),
        Anketa(17,"Anketa 1",date(10,2,2022),date(10,7,2022),25),
        Anketa(18,"Anketa 2",date(10,2,2022),date(10,7,2022),25)
    )
}