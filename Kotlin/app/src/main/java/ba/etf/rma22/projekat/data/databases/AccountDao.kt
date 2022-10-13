package ba.etf.rma22.projekat.data.databases

import androidx.room.*
import ba.etf.rma22.projekat.data.models.Account

@Dao
interface AccountDao {
    @Query("SELECT * FROM account")
    suspend fun getAll(): List<Account>
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAll(vararg accounts: Account)
    @Delete
    suspend fun deleteAll(account: Account)
}