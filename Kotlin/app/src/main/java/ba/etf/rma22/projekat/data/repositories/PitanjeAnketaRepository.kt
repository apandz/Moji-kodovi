package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.data.models.Pitanje
import ba.etf.rma22.projekat.data.models.PitanjeAnketa
import ba.etf.rma22.projekat.data.staticdata.pitanja
import ba.etf.rma22.projekat.data.staticdata.pitanjeAnketa
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object PitanjeAnketaRepository {
    private var odgovori = MutableList<String>(pitanjeAnketa().size) { "" }

    fun getPitanja(nazivAnkete: String, nazivIstrazivanja: String): List<Pitanje> {
        return pitanja().filter { it -> pitanjeAnketa().contains(PitanjeAnketa(it.naziv,nazivAnkete,nazivIstrazivanja)) }
    }

    fun dodajOdgovor(nazivPitanja: String, nazivAnkete: String, nazivIstrazivanja: String, odgovor: String) {
        odgovori[pitanjeAnketa().indexOf(PitanjeAnketa(nazivPitanja,nazivAnkete,nazivIstrazivanja))]=odgovor
    }

    fun getOdgovori(nazivAnkete: String, nazivIstrazivanja: String): List<String> {
        val odgovori1:MutableList<String> = mutableListOf()
        val pitanjeAnketa = pitanjeAnketa()
        for(i in 0..pitanjeAnketa.size-1) {
            if(pitanjeAnketa[i].equals(PitanjeAnketa(pitanjeAnketa[i].naziv,nazivAnkete,nazivIstrazivanja))
                && !odgovori[i].equals("")) {
                odgovori1.add(odgovori[i])
            }
        }
        return odgovori1
    }

    fun getOdgovor(nazivPitanja: String, nazivAnkete: String, nazivIstrazivanja: String): String {
        return odgovori[pitanjeAnketa().indexOf(PitanjeAnketa(nazivPitanja,nazivAnkete,nazivIstrazivanja))]
    }

    suspend fun getPitanja(idAnkete:Int):List<Pitanje>? {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getPitanja(idAnkete).body()
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            if(response!=null)
                for(i in response.indices) {
                db.pitanjeDao().insertAll(response[i])
            }
            return@withContext response
        }
    }
}