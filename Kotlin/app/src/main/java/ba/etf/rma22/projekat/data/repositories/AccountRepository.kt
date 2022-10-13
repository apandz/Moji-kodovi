package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.data.models.Account
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object AccountRepository {
    var acHash="b0449259-36f1-404b-94e9-d2f2855de1a0"

    suspend fun postaviHash(acHash : String) : Boolean {
        val empt= acHash == "b0449259-36f1-404b-94e9-d2f2855de1a0"
        val stariHash = this.acHash
        this.acHash=acHash
        return withContext(Dispatchers.IO) {
            ApiAdapter.retrofit.deleteSve(stariHash)
            val db = AppDatabase.getInstance(MainActivity.context1!!)
            val accs =db.accountDao().getAll()
            if(accs.isNotEmpty()) {
                db.accountDao().deleteAll(accs[0])
                val ankete = db.anketaDao().getAll()
                for(i in ankete.indices) {
                    db.anketaDao().deleteAll(ankete[0])
                }
                val grupe = db.grupaDao().getAll()
                for(i in grupe.indices) {
                    db.grupaDao().deleteAll(grupe[0])
                }
                val odgovori = db.odgovorDao().getAll()
                for(i in odgovori.indices) {
                    db.odgovorDao().deleteAll(odgovori[0])
                }
                val pitanja = db.pitanjeDao().getAll()
                for(i in pitanja.indices) {
                    db.pitanjeDao().deleteAll(pitanja[0])
                }
                val istrazivanja = db.istrazivanjeDao().getAll()
                for(i in istrazivanja.indices) {
                    db.istrazivanjeDao().deleteAll(istrazivanja[0])
                }
            }
            db.accountDao().insertAll(Account(1,"",acHash))
            return@withContext empt
        }
    }
    fun getHash() : String {
        return acHash
    }
}