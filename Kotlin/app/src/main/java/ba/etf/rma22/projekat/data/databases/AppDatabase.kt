package ba.etf.rma22.projekat.data.databases

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.room.TypeConverters
import ba.etf.rma22.projekat.data.models.*

@Database(entities = arrayOf(Odgovor::class, Istrazivanje::class, Grupa::class, Pitanje::class, Anketa::class, Account::class), version = 3)
@TypeConverters(Converters::class)
abstract class AppDatabase : RoomDatabase() {
    abstract fun odgovorDao(): OdgovorDao
    abstract fun istrazivanjeDao(): IstrazivanjeDao
    abstract fun grupaDao(): GrupaDao
    abstract fun pitanjeDao(): PitanjeDao
    abstract fun anketaDao(): AnketaDao
    abstract fun accountDao(): AccountDao

    companion object {
        private var INSTANCE: AppDatabase? = null

        fun getInstance(context: Context): AppDatabase {
            if (INSTANCE == null) {
                synchronized(AppDatabase::class) {
                    INSTANCE = buildRoomDB(context)
                }
            }
            return INSTANCE!!
        }

        private fun buildRoomDB(context: Context) =
            Room.databaseBuilder(
                context.applicationContext,
                AppDatabase::class.java,
                "RMA22DB"
            ).fallbackToDestructiveMigration().build()
    }
}