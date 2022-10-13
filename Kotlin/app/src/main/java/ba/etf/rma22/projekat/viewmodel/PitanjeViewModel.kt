package ba.etf.rma22.projekat.viewmodel

import ba.etf.rma22.projekat.data.models.Pitanje
import ba.etf.rma22.projekat.data.repositories.PitanjeAnketaRepository

class PitanjeViewModel {
    fun getPitanja(nazivAnkete: String, nazivIstrazivanja: String): List<Pitanje> {
        return PitanjeAnketaRepository.getPitanja(nazivAnkete, nazivIstrazivanja)
    }
    fun getOdgovori(nazivAnkete: String, nazivIstrazivanja: String): List<String> {
        return PitanjeAnketaRepository.getOdgovori(nazivAnkete,nazivIstrazivanja)
    }
    fun dodajOdgovor(nazivPitanja: String, nazivAnkete: String, nazivIstrazivanja: String, odgovor: String) {
        PitanjeAnketaRepository.dodajOdgovor(nazivPitanja,nazivAnkete,nazivIstrazivanja,odgovor)
    }
    fun getOdgovor(nazivPitanja: String, nazivAnkete: String, nazivIstrazivanja: String):String {
        return PitanjeAnketaRepository.getOdgovor(nazivPitanja,nazivAnkete,nazivIstrazivanja)
    }
}
