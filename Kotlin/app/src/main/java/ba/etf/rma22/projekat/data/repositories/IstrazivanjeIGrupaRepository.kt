package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.data.models.Grupa
import ba.etf.rma22.projekat.data.models.Istrazivanje
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object IstrazivanjeIGrupaRepository {
    suspend fun getIstrazivanja(offset:Int=0):List<Istrazivanje> {
        return withContext(Dispatchers.IO) {
            var istrazivanja = listOf<Istrazivanje>()
            if(offset==0) {
                val istrazivanja1 = mutableListOf<Istrazivanje>()
                for(i in 1..1000) {
                    val lista = ApiAdapter.retrofit.getIstrazivanja(i).body()
                    if(lista!=null && lista.isNotEmpty()) istrazivanja1.addAll(lista)
                    else break
                }
                istrazivanja=istrazivanja1
            } else {
                val istrazivanja1 = ApiAdapter.retrofit.getIstrazivanja(offset).body()
                if(istrazivanja1!=null) istrazivanja=istrazivanja1
            }
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            for(i in istrazivanja.indices) {
                db.istrazivanjeDao().insertAll(istrazivanja[i])
            }
            return@withContext istrazivanja
        }
    }
    suspend fun getGrupe():List<Grupa> {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getGrupe().body()
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            for(i in response!!.indices) {
                db.grupaDao().insertAll(response[i])
            }
            return@withContext response
        }
    }
    suspend fun getGrupeZaIstrazivanje(idIstrazivanja:Int):List<Grupa> {
        return withContext(Dispatchers.IO) {
            val sveGrupe = ApiAdapter.retrofit.getGrupe().body()
            val grupe = mutableListOf<Grupa>()
            for(i in 0 until sveGrupe!!.size) {
                val response = ApiAdapter.retrofit.getIstrazivanjaZaGrupu(sveGrupe[i].id).body()
                if(response!!.contains(Istrazivanje(idIstrazivanja,"",0))) {
                    grupe.add(sveGrupe[i])
                }
            }
            return@withContext grupe
        }
    }
    suspend fun upisiUGrupu(idGrupa:Int):Boolean {
        return withContext(Dispatchers.IO) {
            try {
                ApiAdapter.retrofit.upisiUGrupu(idGrupa, AccountRepository.getHash())
            } catch (e : Exception) {
                return@withContext false
            }
            return@withContext true
        }
    }
    suspend fun getUpisaneGrupe():List<Grupa> {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getUpisaneGrupe(AccountRepository.getHash()).body()
            return@withContext response!!
        }
    }
}