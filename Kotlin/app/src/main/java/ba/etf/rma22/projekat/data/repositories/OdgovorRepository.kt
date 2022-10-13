package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.data.models.Odgovor
import ba.etf.rma22.projekat.data.models.Podaci
import ba.etf.rma22.projekat.interfaces.Progres
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object OdgovorRepository : Progres {
    suspend fun getOdgovoriAnketa(idAnkete:Int):List<Odgovor> {
        return withContext(Dispatchers.IO) {
            try {
                val poceteAnkete =
                    ApiAdapter.retrofit.getPoceteAnkete(AccountRepository.getHash()).body()
                var idAnketaTaken = 0
                for (i in 0 until poceteAnkete!!.size) {
                    if (poceteAnkete[i].AnketumId == idAnkete) {
                        idAnketaTaken = poceteAnkete[i].id
                        break
                    }
                }
                var response = ApiAdapter.retrofit.getOdgovoriAnketa(
                    AccountRepository.getHash(),
                    idAnketaTaken
                ).body()
                return@withContext response!!
            } catch (e : Exception) {

            }
            return@withContext listOf()
        }
    }

    suspend fun postaviOdgovorAnketa(idAnketaTaken:Int,idPitanje:Int,odgovor:Int):Int {
        return withContext(Dispatchers.IO) {
            try {
                val poceteAnkete = ApiAdapter.retrofit.getPoceteAnkete(AccountRepository.getHash()).body()
                var idAnkete = 0
                for (i in 0 until poceteAnkete!!.size) {
                    if(poceteAnkete[i].id==idAnketaTaken) {
                        idAnkete=poceteAnkete[i].AnketumId
                        break
                    }
                }
                val brojOdgovora = ApiAdapter.retrofit.getOdgovoriAnketa(AccountRepository.getHash(),idAnkete).body()!!.size+1
                val brojPitanja = ApiAdapter.retrofit.getPitanja(idAnkete).body()!!.size
                val progres=noviProgres(brojOdgovora*1.0F/brojPitanja)
                ApiAdapter.retrofit.postaviOdgovorAnketa(AccountRepository.getHash(),idAnketaTaken, Podaci(odgovor,idPitanje,progres))
                val db = AppDatabase.getInstance(MainActivity.context1!!)
                val sviOdg=db.odgovorDao().getAll()
                var idOdg=1
                if(sviOdg.isNotEmpty()) {
                    idOdg=sviOdg[sviOdg.size-1].id+1
                }
                db.odgovorDao().insertAll(Odgovor(idOdg, odgovor))
                return@withContext progres
            } catch (e : Exception) {
                return@withContext -1
            }
        }
    }
}