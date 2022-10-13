package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.data.models.Istrazivanje
import ba.etf.rma22.projekat.data.staticdata.istrazivanja
import kotlin.streams.toList

object IstrazivanjeRepository {
    private var upisanaIstrazivanja: MutableMap<Istrazivanje,String> =
        mapOf(Pair(Istrazivanje(1,"Istraživanje 1/1",1),"Grupa 1"),
            Pair(Istrazivanje(2,"Istraživanje 2/2",2),"Grupa 2"),
            Pair(Istrazivanje(3,"Istraživanje 2/3",3),"Grupa 2")) as MutableMap<Istrazivanje, String>
    private var zadnjaGodina = ""

    fun getIstrazivanjeByGodina(godina:Int): List<Istrazivanje> {
        return istrazivanja().filter { istrazivanje -> istrazivanje.godina==godina}
    }
    fun getAll(): List<Istrazivanje> {
        return istrazivanja()
    }
    fun getUpisani(): List<Istrazivanje> {
        return upisanaIstrazivanja.keys.stream().toList()
    }
    fun upisiIstrazivanje(istrazivanje: String, grupa: String) {
        istrazivanja().find { element -> element.naziv == istrazivanje }
            ?.let { upisanaIstrazivanja.put(it,grupa) }
    }
    fun getUpisanaIstrazivanja(): MutableMap<Istrazivanje,String> {
        return upisanaIstrazivanja
    }
    fun getZadnjaGodina(): String {
        return zadnjaGodina
    }
    fun setZadnjaGodina(zadnjaGodina: String) {
        this.zadnjaGodina= zadnjaGodina
    }
}