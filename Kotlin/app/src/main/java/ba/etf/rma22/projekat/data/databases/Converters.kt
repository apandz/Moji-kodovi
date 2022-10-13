package ba.etf.rma22.projekat.data.databases

import androidx.room.TypeConverter
import java.util.*

class Converters {
    @TypeConverter
    fun fromTimestamp(value: Long?): Date? {
        return value?.let { Date(it) }
    }

    @TypeConverter
    fun dateToTimestamp(date: Date?): Long? {
        return date?.time?.toLong()
    }

    @TypeConverter
    fun listToString(lista: List<String>?): String? {
        var str : String? = null
        if (lista != null) {
            str=""
            for(i in 0 until lista.size) {
                str.plus(lista[i]+"\n")
            }
        }
        return str
    }

    @TypeConverter
    fun stringToList(str: String?): List<String>? {
        var lista : List<String>? = null
        if(str!=null){
            lista=str.split("\n")
        }
        return lista
    }
}
