package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Istrazivanje

@Dao
interface IstrazivanjeDao {
    @Query("SELECT * FROM istrazivanje")
    suspend fun getAll(): List<Istrazivanje>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg istrazivanja: Istrazivanje)
    @Transaction
    @Delete
    suspend fun deleteAll(istrazivanje: Istrazivanje)
}