package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Odgovor

@Dao
interface OdgovorDao {
    @Query("SELECT * FROM odgovor")
    suspend fun getAll(): List<Odgovor>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg odgovori: Odgovor)
    @Transaction
    @Delete
    suspend fun deleteAll(odgovor: Odgovor)
}