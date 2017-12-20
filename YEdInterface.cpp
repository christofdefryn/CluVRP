#include "YEdInterface.h"

using namespace std;

void YEdInterface::preamble(void)
{
	YEdFile_ << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
	YEdFile_ << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\
 xmlns:y=\"http://www.yworks.com/xml/graphml\" xmlns:yed=\"http://www.yworks.com/xml/yed/3\"\
 xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://www.yworks.com/xml/schema/graphml/1.1/ygraphml.xsd\">" << endl;
	YEdFile_ << "	<key for=\"graphml\" id=\"d0\" yfiles.type=\"resources\"/>" << endl;
	YEdFile_ << "	<key for=\"port\" id=\"d1\" yfiles.type=\"portgraphics\"/>" << endl;
	YEdFile_ << "	<key for=\"port\" id=\"d2\" yfiles.type=\"portgeometry\"/>" << endl;
	YEdFile_ << "	<key for=\"port\" id=\"d3\" yfiles.type=\"portuserdata\"/>" << endl;
	YEdFile_ << "	<key attr.name=\"url\" attr.type=\"string\" for=\"node\" id=\"d4\"/>" << endl;
	YEdFile_ << "	<key attr.name=\"description\" attr.type=\"string\" for=\"node\" id=\"d5\"/>" << endl;
	YEdFile_ << "	<key for=\"node\" id=\"d6\" yfiles.type=\"nodegraphics\"/>" << endl;
	YEdFile_ << "	<key attr.name=\"url\" attr.type=\"string\" for=\"edge\" id=\"d7\"/>" << endl;
	YEdFile_ << "	<key attr.name=\"description\" attr.type=\"string\" for=\"edge\" id=\"d8\"/>" << endl;
	YEdFile_ << "	<key for=\"edge\" id=\"d9\" yfiles.type=\"edgegraphics\"/>" << endl;
	YEdFile_ << "	<graph edgedefault=\"undirected\" id=\"G\">" << endl;
}

void YEdInterface::addCluster(Cluster* c)
{
	//YEdFile_ << "		<node id=\"C" << c->getId() << "\" yfiles.foldertype = \"group\">" << endl;

	//YEdFile_ << "		<data key=\"d3\"/>" << endl;
	//YEdFile_ << "		<data key=\"d4\"/>" << endl;
	//YEdFile_ << "		<data key=\"d5\">" << endl;
	//YEdFile_ << "		<y:ProxyAutoBoundsNode>" << endl;
	//YEdFile_ << "		<y:Realizers active=\"0\">" << endl;
	//YEdFile_ << "		<y:GroupNode>" << endl;


	//	<y:Geometry height = "174.0" width = "290.0" x = "835.5" y = "186.5" / >
	//	<y:Fill color = "#F5F5F5" transparent = "false" / >
	//	<y:BorderStyle color = "#000000" type = "dashed" width = "1.0" / >
	//	<y:NodeLabel alignment = "right" autoSizePolicy = "node_width" backgroundColor = "#EBEBEB" borderDistance = "0.0" fontFamily = "Dialog" fontSize = "15" fontStyle = "plain" hasLineColor = "false" height = "22.37646484375" modelName = "internal" modelPosition = "t" textColor = "#000000" visible = "true" width = "290.0" x = "0.0" y = "0.0">Cluster 1< / y:NodeLabel>
	//	<y:Shape type = "roundrectangle" / >
	//	<y:State closed = "false" closedHeight = "50.0" closedWidth = "50.0" innerGraphDisplayEnabled = "false" / >
	//	<y:Insets bottom = "15" bottomF = "15.0" left = "15" leftF = "15.0" right = "15" rightF = "15.0" top = "15" topF = "15.0" / >
	//	<y:BorderInsets bottom = "0" bottomF = "0.0" left = "0" leftF = "0.0" right = "0" rightF = "0.0" top = "0" topF = "0.0" / >
	//	< / y:GroupNode>
	//	<y:GroupNode>
	//	<y:Geometry height = "50.0" width = "50.0" x = "0.0" y = "60.0" / >
	//	<y:Fill color = "#F5F5F5" transparent = "false" / >
	//	<y:BorderStyle color = "#000000" type = "dashed" width = "1.0" / >
	//	<y:NodeLabel alignment = "right" autoSizePolicy = "node_width" backgroundColor = "#EBEBEB" borderDistance = "0.0" fontFamily = "Dialog" fontSize = "15" fontStyle = "plain" hasLineColor = "false" height = "22.37646484375" modelName = "internal" modelPosition = "t" textColor = "#000000" visible = "true" width = "59.02685546875" x = "-4.513427734375" y = "0.0">Folder 1< / y:NodeLabel>
	//	<y:Shape type = "roundrectangle" / >
	//	<y:State closed = "true" closedHeight = "50.0" closedWidth = "50.0" innerGraphDisplayEnabled = "false" / >
	//	<y:Insets bottom = "5" bottomF = "5.0" left = "5" leftF = "5.0" right = "5" rightF = "5.0" top = "5" topF = "5.0" / >
	//	<y:BorderInsets bottom = "0" bottomF = "0.0" left = "0" leftF = "0.0" right = "0" rightF = "0.0" top = "0" topF = "0.0" / >
	//	< / y:GroupNode>
	//	< / y:Realizers>
	//	< / y:ProxyAutoBoundsNode>
	//	< / data>
	//	<graph edgedefault = "directed" id = "n0:" / >
	//	< / node>
}

void YEdInterface::drawNodes(CluVRPinst*& cluVRPinst)
{
	Cluster* c = nullptr;
	Node* n = nullptr;

	for (int i = 0; i < cluVRPinst->getnClusters(); i++)
	{
		c = cluVRPinst->getCluster(i);
		
		for (int j = 0; j < c->getnNodes(); j++)
		{
			n = c->getNode(j);

			YEdFile_ << "	<node id=\"n" << n->id << "\">" << endl;
			YEdFile_ << "		<data key=\"d5\"/>" << endl;
			YEdFile_ << "		<data key=\"d6\">" << endl;
			YEdFile_ << "			<y:ShapeNode>" << endl;
			if (c->isDepot) YEdFile_ << "				<y:Geometry height=\"25\" width=\"25\" x=\"" << n->x * 10 << "\" y=\"" << -n->y * 10<< "\"/>" << endl;
			else YEdFile_ << "				<y:Geometry height=\"10\" width=\"10\" x=\"" << n->x * 10<< "\" y=\"" << -n->y * 10<< "\"/>" << endl;
			YEdFile_ << "				<y:Fill color=\"#000000\" transparent=\"false\"/>" << endl;
			YEdFile_ << "				<y:BorderStyle color=\"#000000\" type=\"line\" width=\"0.6\"/>" << endl;

			//add node label
			YEdFile_ << "				<y:NodeLabel alignment=\"center\" autoSizePolicy=\"content\" fontFamily=\"Dialog\" fontSize=\"18\" fontStyle=\"plain\" hasBackgroundColor=\"false\" hasLineColor=\"false\" height=\"26\" modelName=\"eight_pos\" modelPosition=\"n\" textColor=\"#000000\" visible=\"true\" width=\"14\" x=\"-2.0\" y=\"-30.0\">" << n->id << "</y:NodeLabel>" << endl;

			//shape
			if (c->isDepot) YEdFile_ << "				<y:Shape type=\"rectangle\"/>" << endl;
			else YEdFile_ << "				<y:Shape type=\"circle\"/>" << endl;

			YEdFile_ << "			</y:ShapeNode>" << endl;
			YEdFile_ << "		</data>" << endl;
			YEdFile_ << "	</node>" << endl;

		}
	}
}

void YEdInterface::drawSolution(NodeSolution*& s, int& edgeId)
{
	for (int i = 0; i < s->getnTrips(); i++)
	{
		NodTrip* t = s->getTrip(i);

		for (int j = 0; j < t->getSize() - 1; j++)
		{
			YEdFile_ << "	<edge id=\"e"<< edgeId << "\" source=\"n" << t->getNode(j)->id << "\" target=\"n" << t->getNode(j + 1)->id << "\">" << endl;
			YEdFile_ << "		<data key=\"d8\"/>" << endl;
			YEdFile_ << "		<data key=\"d9\">" << endl;
			YEdFile_ << "		<y:PolyLineEdge>" << endl;
			YEdFile_ << "			<y:Path sx=\"0.0\" sy=\"0.0\" tx=\"0.0\" ty=\"0.0\">" << endl;
			//YEdFile_ << "				<y:Point x=\"600.0\" y=\"-300.0\"/>" << endl;
			YEdFile_ << "			</y:Path>" << endl;
			YEdFile_ << "			<y:LineStyle color=\"#000000\" type=\"line\" width=\"1.0\"/>" << endl;
			YEdFile_ << "			<y:Arrows source=\"none\" target=\"standard\"/>" << endl;
			YEdFile_ << "			<y:BendStyle smoothed=\"false\"/>" << endl;
			YEdFile_ << "		</y:PolyLineEdge>" << endl;
			YEdFile_ << "		</data>" << endl;
			YEdFile_ << "	</edge>" << endl;

			edgeId++;
		}
	}
}

void YEdInterface::closeFile(void)
{
	YEdFile_ << "	</graph>" << endl;
	YEdFile_ << "	<data key=\"d6\">" << endl;
	YEdFile_ << "		<y:Resources/>" << endl;
	YEdFile_ << "	</data>" << endl;
	YEdFile_ << "</graphml>" << endl;

	YEdFile_.close();
}

YEdInterface::YEdInterface(){}

YEdInterface::~YEdInterface(){}

void YEdInterface::generateYEdInstance(CluVRPinst*& cluVRPdata, CluVRPsol*& cluVRPsol, std::string& instance)
{
	YEdFile_.open("yEd-output/" + instance + ".graphml");

	if (YEdFile_.is_open())
	{
		preamble();
		drawNodes(cluVRPdata);

		int edgeId = 1;
		
		drawSolution(cluVRPsol->sNode_, edgeId);

		closeFile();
	}
}