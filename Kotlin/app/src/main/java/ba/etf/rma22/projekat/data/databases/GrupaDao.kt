package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Grupa

@Dao
interface GrupaDao {
    @Query("SELECT * FROM grupa")
    suspend fun getAll(): List<Grupa>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg grupe: Grupa)
    @Transaction
    @Delete
    suspend fun deleteAll(grupa: Grupa)
}