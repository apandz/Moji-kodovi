package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Pitanje

@Dao
interface PitanjeDao {
    @Query("SELECT * FROM pitanje")
    suspend fun getAll(): List<Pitanje>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg pitanja: Pitanje)
    @Transaction
    @Delete
    suspend fun deleteAll(pitanje: Pitanje)
}