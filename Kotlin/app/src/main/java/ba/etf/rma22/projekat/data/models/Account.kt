package ba.etf.rma22.projekat.data.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey
import com.google.gson.annotations.SerializedName

@Entity
data class Account(
    @PrimaryKey @ColumnInfo(name = "id") @SerializedName("id") val id : Int,
    @ColumnInfo(name = "student") @SerializedName("student") val student : String,
    @ColumnInfo(name = "acHash") @SerializedName("acHash") val acHash : String
)
