package ba.etf.rma22.projekat

import android.content.Context
import android.net.Uri
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import androidx.appcompat.app.AppCompatActivity
import androidx.viewpager2.widget.ViewPager2
import ba.etf.rma22.projekat.data.databases.AppDatabase
import ba.etf.rma22.projekat.fragments.FragmentAnkete
import ba.etf.rma22.projekat.fragments.FragmentIstrazivanje
import ba.etf.rma22.projekat.fragments.FragmentPoruka
import ba.etf.rma22.projekat.interfaces.Communicator
import ba.etf.rma22.projekat.view.ViewPagerAdapter
import ba.etf.rma22.projekat.viewmodel.AccountViewModel

class MainActivity : AppCompatActivity(), Communicator {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        viewPager = findViewById(R.id.pager)
        val fragments =
            mutableListOf(
                FragmentAnkete(),
                FragmentIstrazivanje()
            )

        viewPager.offscreenPageLimit = 3
        viewPagerAdapter = ViewPagerAdapter(supportFragmentManager, fragments, lifecycle)
        viewPager.adapter = viewPagerAdapter

        context1=baseContext
        AppDatabase.getInstance(context1!!)

        val uri: Uri? = intent.data

        if (uri != null) {
            val parameters: List<String> = uri.pathSegments
            val param = parameters[parameters.size - 1]
            val avm = AccountViewModel()
            avm.postaviHash(param)
        }
    }

    override fun passDataCom(text: String) {
        val bundle = Bundle()
        bundle.putString("message",text)

        val fragmentPoruka = FragmentPoruka()
        fragmentPoruka.arguments=bundle

        Handler(Looper.getMainLooper()).postDelayed({
            viewPagerAdapter.refreshFragment(1, fragmentPoruka)
        }, 100)
    }

    companion object {
        lateinit var viewPagerAdapter: ViewPagerAdapter
        lateinit var viewPager: ViewPager2
        var context1 : Context? = null
    }
}