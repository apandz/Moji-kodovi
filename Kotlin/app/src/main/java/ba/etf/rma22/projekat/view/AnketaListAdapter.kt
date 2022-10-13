package ba.etf.rma22.projekat.view

import android.icu.text.SimpleDateFormat
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.ProgressBar
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import ba.etf.rma22.projekat.R
import ba.etf.rma22.projekat.data.models.Anketa
import ba.etf.rma22.projekat.interfaces.Progres
import ba.etf.rma22.projekat.viewmodel.PitanjeViewModel
import java.util.*

class AnketaListAdapter(
    private var ankete: List<Anketa>
): RecyclerView.Adapter<AnketaListAdapter.AnketaViewHolder>(), Progres {
    var onItemClick: ((Anketa) -> Unit)? = null
    private var pitanjeViewModel = PitanjeViewModel()

    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): AnketaViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_anketa, parent, false)
        return AnketaViewHolder(view)
    }
    init { ankete=sort(ankete) }

    private fun sort(ankete: List<Anketa>): List<Anketa> {
        return ankete.sortedWith { anketa1: Anketa, anketa2: Anketa ->
            anketa1.datumPocetak.compareTo(
                anketa2.datumPocetak
            )
        }
    }

    override fun getItemCount(): Int = ankete.size
    override fun onBindViewHolder(holder: AnketaViewHolder, position: Int) {

        var progres = pitanjeViewModel.getOdgovori(ankete[position].naziv,/*ankete[position].nazivIstrazivanja*/"Naziv istraživanja").size*1.0F/
                pitanjeViewModel.getPitanja(ankete[position].naziv,/*ankete[position].nazivIstrazivanja*/"Naziv istraživanja").size

        holder.nazivAnkete.text=ankete[position].naziv
        holder.nazivIstrazivanja.text=/*ankete[position].nazivIstrazivanja*/"Naziv istraživanja"
        val formatter = SimpleDateFormat("dd.MM.yyyy")
        if(ankete[position].datumKraj!=null) {
            val date: String = formatter.format(ankete[position].datumKraj)
            holder.statusAnkete.setImageResource(R.drawable.plava)
            holder.datumAnkete.text="Anketa urađena: "+date
        } else {
            val trenutniDatum: Date = Calendar.getInstance().time
            if(ankete[position].datumPocetak.after(trenutniDatum)) {
                val date: String = formatter.format(ankete[position].datumPocetak)
                holder.statusAnkete.setImageResource(R.drawable.zuta)
                holder.datumAnkete.text="Vrijeme aktiviranja: "+date
                progres = 0F
            } else {
                /*if(ankete[position].datumKraj.before(trenutniDatum)) {
                    val date: String = formatter.format(ankete[position].datumKraj)
                    holder.statusAnkete.setImageResource(R.drawable.crvena)
                    holder.datumAnkete.text="Anketa zatvorena: "+date
                } else {*/
                    if(ankete[position].datumKraj!=null) {
                        val date: String = formatter.format(ankete[position].datumKraj)
                        holder.statusAnkete.setImageResource(R.drawable.zelena)
                        holder.datumAnkete.text = "Vrijeme zatvaranja: " + date
                    } else {
                        holder.statusAnkete.setImageResource(R.drawable.zelena)
                        holder.datumAnkete.text = "Vrijeme zatvaranja: /"
                    }
                /*}*/
            }
        }

        holder.progresZavrsetka.progress = noviProgres(progres)
    }
    fun updateAnkete(ankete: List<Anketa>) {
        this.ankete=sort(ankete)
        notifyDataSetChanged()
    }
    inner class AnketaViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val nazivIstrazivanja: TextView = itemView.findViewById(R.id.nazivIstrazivanja)
        val nazivAnkete: TextView = itemView.findViewById(R.id.nazivAnkete)
        val datumAnkete: TextView = itemView.findViewById(R.id.datumAnkete)
        val statusAnkete: ImageView = itemView.findViewById(R.id.statusAnkete)
        val progresZavrsetka: ProgressBar = itemView.findViewById(R.id.progresZavrsetka)

        init {
            itemView.setOnClickListener {
                onItemClick?.invoke(ankete[adapterPosition])
            }
        }
    }
}