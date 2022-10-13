package ba.etf.rma22.projekat.fragments

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView.OnItemClickListener
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.ListView
import android.widget.TextView
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import ba.etf.rma22.projekat.MainActivity
import ba.etf.rma22.projekat.R
import ba.etf.rma22.projekat.data.models.Anketa
import ba.etf.rma22.projekat.data.models.Pitanje
import ba.etf.rma22.projekat.viewmodel.PitanjeViewModel
import java.util.*

class FragmentPitanje : Fragment() {
    private lateinit var tekstPitanja: TextView
    private lateinit var odgovoriLista: ListView
    private lateinit var dugmeZaustavi: Button
    private var pitanjeViewModel = PitanjeViewModel()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_pitanje, container, false)

        val pitanje = requireArguments().getSerializable("pitanje") as Pitanje
        val anketa = requireArguments().getSerializable("anketa") as Anketa

        tekstPitanja=view.findViewById(R.id.tekstPitanja)
        odgovoriLista=view.findViewById(R.id.odgovoriLista)
        dugmeZaustavi=view.findViewById(R.id.dugmeZaustavi)

        tekstPitanja.text = pitanje.tekstPitanja

        val itemsAdapter = MyArrayAdapter(activity as Context, android.R.layout.simple_list_item_1, pitanje.opcije)
        odgovoriLista.adapter = itemsAdapter

        val trenutniDatum: Date = Calendar.getInstance().time
        if(anketa.datumKraj!=null && anketa.datumKraj.before(trenutniDatum))
            odgovoriLista.isEnabled=false
        odgovoriLista.onItemClickListener=OnItemClickListener {
                parent, view, position, id ->
            for(i in 0..odgovoriLista.childCount-1) {
                if(i==position){
                        (parent.getChildAt(i) as TextView).setTextColor(ContextCompat.getColor(activity as Context, R.color.blue))
                } else {
                        (parent.getChildAt(i) as TextView).setTextColor(ContextCompat.getColor(activity as Context, R.color.defaultText))
                }
            }
            pitanjeViewModel.dodajOdgovor(pitanje.naziv,anketa.naziv,"naziv",(parent.getChildAt(position) as TextView).text.toString())
        }

        dugmeZaustavi.setOnClickListener { _ ->
            Handler(Looper.getMainLooper()).postDelayed({
                MainActivity.viewPagerAdapter.defaultFragmenti()
                MainActivity.viewPager.currentItem = 0
            }, 500)
        }

        return view
    }

    inner class MyArrayAdapter(context: Context, private val layoutResource: Int, private val elements: List<String>):
        ArrayAdapter<String>(context, layoutResource, elements) {
        override fun getView(position: Int, convertView: View?, parent: ViewGroup): View {
            val view = super.getView(position, convertView, parent)

            val textView = view.findViewById<TextView>(android.R.id.text1)

            val pitanje = requireArguments().getSerializable("pitanje") as Pitanje
            val anketa = requireArguments().getSerializable("anketa") as Anketa

            if(textView.text.equals(pitanjeViewModel.getOdgovor(pitanje.naziv,anketa.naziv,"naziv")))
                textView.setTextColor(ContextCompat.getColor(activity as Context, R.color.blue))

            return view
        }
        }

    companion object {
        fun newInstance() = FragmentPitanje()
    }
}