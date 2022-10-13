package ba.unsa.etf.rpr;

import net.sf.jasperreports.engine.*;
import net.sf.jasperreports.view.JasperViewer;

import javax.swing.*;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.HashMap;

public class GradoviReport extends JFrame {
    public void showReport(Connection conn, int jezik) throws JRException {
        String reportSrcFile = getClass().getResource("/reports/gradovi.jrxml").getFile();
        String reportsDir = getClass().getResource("/reports/").getFile();

        JasperReport jasperReport = JasperCompileManager.compileReport(reportSrcFile);
        HashMap<String, Object> parameters = new HashMap<String, Object>();
        parameters.put("reportsDirPath", reportsDir);

        String izvjestaj="Izvještaj", gradovi="Gradovi", naziv="Naziv", broj_stanovnika="Broj stanovnika", drzava="Država", stranica="Stranica";
        if(jezik==2) {
            izvjestaj="Report";
            gradovi="Cities";
            naziv="Name";
            broj_stanovnika="Population";
            drzava="Country";
            stranica="Page";
        } else if(jezik==3) {
            izvjestaj="Bericht";
            gradovi="Städte";
            naziv="Name";
            broj_stanovnika="Bevölkerung";
            drzava="Land";
            stranica="Seite";
        } else if(jezik==4) {
            izvjestaj="Rapport";
            gradovi="Villes";
            naziv="Nom";
            broj_stanovnika="Population";
            drzava="Pays";
            stranica="Page";
        }
        parameters.remove("izvjestaj");
        parameters.remove("gradovi");
        parameters.remove("naziv");
        parameters.remove("broj_stanovnika");
        parameters.remove("drzava");
        parameters.remove("stranica");
        parameters.put("izvjestaj",izvjestaj);
        parameters.put("gradovi",gradovi);
        parameters.put("naziv",naziv);
        parameters.put("broj_stanovnika",broj_stanovnika);
        parameters.put("drzava",drzava);
        parameters.put("stranica",stranica);

        ArrayList<HashMap<String, Object>> list = new ArrayList<HashMap<String, Object>>();
        list.add(parameters);

        JasperPrint print = JasperFillManager.fillReport(jasperReport, parameters, conn);
        JasperViewer viewer = new JasperViewer(print, false);
        viewer.setVisible(true);
    }
}
