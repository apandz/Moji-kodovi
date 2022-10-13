package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Anketa

@Dao
interface AnketaDao {
    @Query("SELECT * FROM anketa")
    suspend fun getAll(): List<Anketa>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg ankete: Anketa)
    @Transaction
    @Delete
    suspend fun deleteAll(anketa: Anketa)
}