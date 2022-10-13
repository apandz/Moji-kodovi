package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.data.models.AnketaTaken
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object TakeAnketaRepository {

    suspend fun zapocniAnketu(idAnkete:Int): AnketaTaken? {
        return withContext(Dispatchers.IO) {
            try {
                val response = ApiAdapter.retrofit.zapocniAnketu(AccountRepository.getHash(),idAnkete)
                return@withContext response.body()
            } catch (e : Exception) {

            }
            return@withContext null
        }
    }
    suspend fun getPoceteAnkete():List<AnketaTaken>? {
        return withContext(Dispatchers.IO) {
            val response = ApiAdapter.retrofit.getPoceteAnkete(AccountRepository.getHash()).body()
            if(response!!.isEmpty()) return@withContext null
            return@withContext response
        }
    }
}