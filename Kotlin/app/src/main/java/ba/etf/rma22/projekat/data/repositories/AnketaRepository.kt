package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.data.models.Anketa
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object AnketaRepository {
    suspend fun getMyAnkete(): List<Anketa> {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getUpisaneGrupe(AccountRepository.getHash()).body()
            val ankete = mutableListOf<Anketa>()
            for(i in 0 until response!!.size) {
                val ankete1 = ApiAdapter.retrofit.getUpisane(response[i].id).body()
                ankete.addAll(ankete1!!)
            }
            return@withContext ankete
        }
    }
    suspend fun getAll(): List<Anketa> {
        return withContext(Dispatchers.IO) {
            val ankete = mutableListOf<Anketa>()
            for(i in 1..1000) {
                val response = ApiAdapter.retrofit.getAll(i).body()
                if(response==null || response.isEmpty()) {
                    break
                } else {
                    ankete.addAll(response)
                }
            }
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            for(i in ankete.indices) {
                db.anketaDao().insertAll(ankete[i])
            }
            return@withContext ankete
        }
    }
    suspend fun getDone(): List<Anketa> {
        return withContext(Dispatchers.IO) {
            val ankete = mutableListOf<Anketa>()
            for(i in 1..1000) {
                val response = ApiAdapter.retrofit.getAll(i).body()
                if(response==null || response.isEmpty()) {
                    break
                } else {
                    ankete.addAll(response)
                }
            }
            return@withContext ankete
        }
    }
    suspend fun getFuture(): List<Anketa> {
        return withContext(Dispatchers.IO) {
            val ankete = mutableListOf<Anketa>()
            for(i in 1..1000) {
                val response = ApiAdapter.retrofit.getAll(i).body()
                if(response==null || response.isEmpty()) {
                    break
                } else {
                    ankete.addAll(response)
                }
            }
            return@withContext ankete
        }
    }
    suspend fun getNotTaken(): List<Anketa> {
        return withContext(Dispatchers.IO) {
            val ankete = mutableListOf<Anketa>()
            for(i in 1..1000) {
                val response = ApiAdapter.retrofit.getAll(i).body()
                if(response==null || response.isEmpty()) {
                    break
                } else {
                    ankete.addAll(response)
                }
            }
            return@withContext ankete
        }
    }

    suspend fun getAll(offset:Int):List<Anketa> {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getAll(offset).body()
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            for(i in response!!.indices) {
                db.anketaDao().insertAll(response[i])
            }
            return@withContext response
        }
    }
    suspend fun getById(id:Int):Anketa? {
        return withContext(Dispatchers.IO) {
            try {
                val response = ApiAdapter.retrofit.getById(id)
                return@withContext response.body()!!
            } catch (e : Exception) {

            }
            return@withContext null
        }
    }
    suspend fun getUpisane():List<Anketa> {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getUpisaneGrupe(AccountRepository.getHash()).body()
            val ankete = mutableListOf<Anketa>()
            for(i in 0 until response!!.size) {
                val ankete1 = ApiAdapter.retrofit.getUpisane(response[i].id).body()
                ankete.addAll(ankete1!!)
            }
            return@withContext ankete
        }
    }
}