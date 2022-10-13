package ba.etf.rma22.projekat.viewmodel

import ba.etf.rma22.projekat.data.models.Istrazivanje
import ba.etf.rma22.projekat.data.repositories.IstrazivanjeRepository

class IstrazivanjeViewModel {
    fun getIstrazivanjeByGodina(godina:Int): List<Istrazivanje> {
        return IstrazivanjeRepository.getIstrazivanjeByGodina(godina).filter{ istrazivanje -> !IstrazivanjeRepository.getUpisanaIstrazivanja().contains(istrazivanje)}
    }
    fun getAll(): List<Istrazivanje> {
        return IstrazivanjeRepository.getAll()
    }
    fun getUpisani(): List<Istrazivanje> {
        return IstrazivanjeRepository.getUpisani()
    }
    fun upisiIstrazivanje(istrazivanje: String, grupa: String) {
        IstrazivanjeRepository.upisiIstrazivanje(istrazivanje,grupa)
    }
    fun getUpisanaIstrazivanja(): MutableMap<Istrazivanje,String> {
        return IstrazivanjeRepository.getUpisanaIstrazivanja()
    }
    fun getZadnjaGodina(): String {
        return IstrazivanjeRepository.getZadnjaGodina()
    }
    fun setZadnjaGodina(zadnjaGodina: String) {
        IstrazivanjeRepository.setZadnjaGodina(zadnjaGodina)
    }
}