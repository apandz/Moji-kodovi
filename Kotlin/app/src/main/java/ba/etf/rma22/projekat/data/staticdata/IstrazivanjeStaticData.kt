package ba.etf.rma22.projekat.data.staticdata

import ba.etf.rma22.projekat.data.models.Istrazivanje

fun istrazivanja(): List<Istrazivanje> {
    return listOf(
        Istrazivanje(1,"Istraživanje 1/1",1),
        Istrazivanje(2,"Istraživanje 2/1",1),
        Istrazivanje(3,"Istraživanje 1/2",2),
        Istrazivanje(4,"Istraživanje 2/2",2),
        Istrazivanje(5,"Istraživanje 1/3",3),
        Istrazivanje(6,"Istraživanje 2/3",3),
        Istrazivanje(7,"Istraživanje 1/4",4),
        Istrazivanje(8,"Istraživanje 1/5",5)
    )
}